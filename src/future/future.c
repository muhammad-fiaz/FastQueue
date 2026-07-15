/**
 * @file future.c
 * @brief Future / promise implementation with spin-then-wait strategy.
 */

#include "fastqueue/future.h"
#include "fastqueue/memory.h"
#include "fastqueue/atomic.h"
#include "fastqueue/config.h"
#include "fastqueue/platform.h"

#include <string.h>

/* ── Internal structure ──────────────────────────────────────────────── */

struct fq_future_t {
    fq_atomic_int_t     ready;      /* 0 = pending, 1 = fulfilled */
    fq_atomic_int_t     canceled;
    fq_status_t         status;     /* only valid when ready == 1 */
    fq_completion_fn    callback;
    void               *callback_data;
    fq_mutex_t          mutex;
    fq_condition_t      cond;
    const fq_allocator_t *allocator;
};

/* ── Creation / destruction ──────────────────────────────────────────── */

fq_status_t fq_future_create(fq_future_t **future,
                             const fq_allocator_t *allocator)
{
    if (!future) return FQ_ERR_INVAL;
    if (!allocator) allocator = fq_default_allocator();

    fq_future_t *f = fq_calloc(allocator, 1, sizeof(*f));
    if (!f) return FQ_ERR_NOMEM;

    fq_atomic_store_explicit(&f->ready, 0, FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_store_explicit(&f->canceled, 0, FQ_MEMORY_ORDER_RELAXED);
    f->status         = FQ_OK;
    f->callback       = NULL;
    f->callback_data  = NULL;
    f->allocator      = allocator;

    if (fq_mutex_init(&f->mutex) != 0) {
        fq_free(allocator, f);
        return FQ_ERR_INTERNAL;
    }
    if (fq_condition_init(&f->cond) != 0) {
        fq_mutex_destroy(&f->mutex);
        fq_free(allocator, f);
        return FQ_ERR_INTERNAL;
    }

    *future = f;
    return FQ_OK;
}

void fq_future_destroy(fq_future_t *future)
{
    if (!future) return;

    /* If still pending, cancel it. */
    if (!fq_future_is_ready(future)) {
        fq_future_cancel(future);
    }

    fq_condition_destroy(&future->cond);
    fq_mutex_destroy(&future->mutex);
    fq_free(future->allocator, future);
}

/* ── Waiting ─────────────────────────────────────────────────────────── */

fq_status_t fq_future_wait(fq_future_t *future)
{
    if (!future) return FQ_ERR_INVAL;

    /* Spin phase. */
    for (unsigned i = 0; i < FQ_FUTURE_SPIN_COUNT; ++i) {
        if (fq_atomic_load_explicit(&future->ready, FQ_MEMORY_ORDER_ACQUIRE))
            return future->status;
        fq_thread_yield();
    }

    /* Wait phase. */
    fq_mutex_lock(&future->mutex);
    while (!fq_atomic_load_explicit(&future->ready, FQ_MEMORY_ORDER_RELAXED)) {
        fq_condition_wait(&future->cond, &future->mutex);
    }
    fq_mutex_unlock(&future->mutex);

    return future->status;
}

fq_status_t fq_future_wait_timeout(fq_future_t *future,
                                   unsigned timeout_ms,
                                   fq_status_t *status)
{
    if (!future) return FQ_ERR_INVAL;

    /* Spin phase (shorter). */
    unsigned spin = timeout_ms > 0 ? FQ_FUTURE_SPIN_COUNT / 4 : 0;
    for (unsigned i = 0; i < spin; ++i) {
        if (fq_atomic_load_explicit(&future->ready, FQ_MEMORY_ORDER_ACQUIRE)) {
            if (status) *status = future->status;
            return FQ_OK;
        }
        fq_thread_yield();
    }

    /* Wait phase with timeout. */
    fq_mutex_lock(&future->mutex);
    while (!fq_atomic_load_explicit(&future->ready, FQ_MEMORY_ORDER_RELAXED)) {
        if (fq_condition_timedwait(&future->cond, &future->mutex,
                                   timeout_ms) != 0) {
            fq_mutex_unlock(&future->mutex);
            return FQ_ERR_TIMEOUT;
        }
    }
    fq_mutex_unlock(&future->mutex);

    if (status) *status = future->status;
    return FQ_OK;
}

/* ── Queries ─────────────────────────────────────────────────────────── */

fq_bool_t fq_future_is_ready(const fq_future_t *future)
{
    if (!future) return FQ_FALSE;
    return fq_atomic_load_explicit(
               &((fq_future_t *)future)->ready, FQ_MEMORY_ORDER_ACQUIRE)
                   ? FQ_TRUE
                   : FQ_FALSE;
}

fq_bool_t fq_future_is_canceled(const fq_future_t *future)
{
    if (!future) return FQ_FALSE;
    return fq_atomic_load_explicit(
               &((fq_future_t *)future)->canceled, FQ_MEMORY_ORDER_ACQUIRE)
                   ? FQ_TRUE
                   : FQ_FALSE;
}

fq_status_t fq_future_status(const fq_future_t *future)
{
    if (!future) return FQ_ERR_INVAL;
    if (!fq_future_is_ready(future)) return FQ_ERR_BUSY;
    return future->status;
}

/* ── Completion callback ─────────────────────────────────────────────── */

fq_status_t fq_future_on_complete(fq_future_t *future,
                                  fq_completion_fn callback,
                                  void *user_data)
{
    if (!future || !callback) return FQ_ERR_INVAL;

    fq_mutex_lock(&future->mutex);
    future->callback      = callback;
    future->callback_data = user_data;

    /* If already fulfilled, invoke immediately. */
    if (fq_atomic_load_explicit(&future->ready, FQ_MEMORY_ORDER_RELAXED)) {
        fq_mutex_unlock(&future->mutex);
        callback(user_data, future->status);
        return FQ_OK;
    }
    fq_mutex_unlock(&future->mutex);
    return FQ_OK;
}

/* ── Cancel ──────────────────────────────────────────────────────────── */

fq_status_t fq_future_cancel(fq_future_t *future)
{
    if (!future) return FQ_ERR_INVAL;

    int expected = 0;
    if (!fq_atomic_compare_exchange_strong_explicit(
            &future->canceled, &expected, 1,
            FQ_MEMORY_ORDER_ACQ_REL, FQ_MEMORY_ORDER_RELAXED)) {
        return FQ_OK; /* already canceled */
    }

    fq_future_set_result(future, FQ_ERR_CANCELED);
    return FQ_OK;
}

/* ── Set result (called by scheduler / task) ─────────────────────────── */

void fq_future_set_result(fq_future_t *future, fq_status_t status)
{
    if (!future) return;

    fq_mutex_lock(&future->mutex);
    future->status = status;
    fq_atomic_store_explicit(&future->ready, 1, FQ_MEMORY_ORDER_RELEASE);
    fq_condition_broadcast(&future->cond);
    fq_mutex_unlock(&future->mutex);

    /* Invoke completion callback if registered. */
    if (future->callback) {
        future->callback(future->callback_data, status);
    }
}
