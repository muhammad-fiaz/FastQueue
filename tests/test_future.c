/**
 * @file test_future.c
 * @brief Future unit tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

int test_future_wait(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    if (fq_future_is_ready(f)) return 2;

    fq_future_set_result(f, FQ_OK);

    if (!fq_future_is_ready(f)) return 3;
    if (fq_future_status(f) != FQ_OK) return 4;

    fq_future_destroy(f);
    return 0;
}

int test_future_timeout(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    fq_status_t status;
    fq_status_t st = fq_future_wait_timeout(f, 10, &status);
    if (st != FQ_ERR_TIMEOUT) { fq_future_destroy(f); return 2; }

    fq_future_destroy(f);
    return 0;
}

int test_future_cancel(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    fq_status_t st = fq_future_cancel(f);
    if (st != FQ_OK) return 2;
    if (!fq_future_is_canceled(f)) return 3;
    if (!fq_future_is_ready(f)) return 4;

    fq_future_destroy(f);
    return 0;
}

static int g_complete_called = 0;
static fq_status_t g_complete_status;

static void on_complete_cb(void *user_data, fq_status_t status)
{
    (void)user_data;
    g_complete_called = 1;
    g_complete_status = status;
}

int test_future_on_complete(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    g_complete_called = 0;
    g_complete_status = FQ_OK;

    /* Register callback before fulfillment. */
    if (fq_future_on_complete(f, on_complete_cb, NULL) != FQ_OK) {
        fq_future_destroy(f);
        return 2;
    }

    /* Fulfill the future. */
    fq_future_set_result(f, FQ_OK);

    /* Give callback time to fire. */
    fq_thread_sleep_ms(10);

    if (!g_complete_called) {
        fq_future_destroy(f);
        return 3;
    }
    if (g_complete_status != FQ_OK) {
        fq_future_destroy(f);
        return 4;
    }

    fq_future_destroy(f);
    return 0;
}

int test_future_on_complete_already_ready(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    g_complete_called = 0;
    g_complete_status = FQ_OK;

    /* Fulfill first, then register callback. */
    fq_future_set_result(f, FQ_OK);

    if (fq_future_on_complete(f, on_complete_cb, NULL) != FQ_OK) {
        fq_future_destroy(f);
        return 2;
    }

    /* Callback should fire immediately. */
    if (!g_complete_called) {
        fq_future_destroy(f);
        return 3;
    }
    if (g_complete_status != FQ_OK) {
        fq_future_destroy(f);
        return 4;
    }

    fq_future_destroy(f);
    return 0;
}

int test_future_on_complete_with_status(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    g_complete_called = 0;
    g_complete_status = FQ_OK;

    if (fq_future_on_complete(f, on_complete_cb, NULL) != FQ_OK) {
        fq_future_destroy(f);
        return 2;
    }

    fq_future_set_result(f, FQ_ERR_TIMEOUT);
    fq_thread_sleep_ms(10);

    if (!g_complete_called) {
        fq_future_destroy(f);
        return 3;
    }
    if (g_complete_status != FQ_ERR_TIMEOUT) {
        fq_future_destroy(f);
        return 4;
    }

    fq_future_destroy(f);
    return 0;
}

static fq_atomic_int_t g_future_wait_counter = FQ_ATOMIC_INIT(0);

static void delayed_set_task(void *arg)
{
    fq_future_t *f = (fq_future_t *)arg;
    fq_thread_sleep_ms(50);
    fq_future_set_result(f, FQ_OK);
    fq_atomic_fetch_add(&g_future_wait_counter, 1);
}

int test_future_wait_blocking(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_future_t *future = NULL;
    fq_task_t *task = NULL;
    if (fq_task_create(&task, delayed_set_task, NULL, NULL) != FQ_OK) {
        fq_scheduler_shutdown(s);
        return 2;
    }

    if (fq_scheduler_submit_with_future(s, task, &future) != FQ_OK) {
        fq_task_destroy(task);
        fq_scheduler_shutdown(s);
        return 3;
    }

    /* This should block until the task completes. */
    fq_status_t result = fq_future_wait(future);
    if (result != FQ_OK) {
        fq_future_destroy(future);
        fq_scheduler_shutdown(s);
        return 4;
    }

    if (!fq_future_is_ready(future)) {
        fq_future_destroy(future);
        fq_scheduler_shutdown(s);
        return 5;
    }

    fq_future_destroy(future);
    fq_scheduler_shutdown(s);
    return 0;
}
