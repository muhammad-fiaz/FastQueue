/**
 * @file scheduler.c
 * @brief Job scheduler with multiple worker threads.
 *
 * The scheduler maintains an array of per-thread queues and a global
 * queue.  Worker threads first drain their local queue, then attempt
 * work-stealing from other threads, and finally fall back to the global
 * queue.
 */

#include "fastqueue/scheduler.h"
#include "fastqueue/task.h"
#include "fastqueue/future.h"
#include "fastqueue/queue.h"
#include "fastqueue/memory.h"
#include "fastqueue/atomic.h"
#include "fastqueue/config.h"
#include "fastqueue/platform.h"
#include "fastqueue/time.h"

#include <string.h>
#include <stdio.h>

 
void fq_task_execute(fq_task_t *task);

 

typedef struct fq_worker_t {
    fq_thread_t     thread;
    fq_queue_t     *queue;
    unsigned        id;
    fq_bool_t       running;
    fq_scheduler_t *scheduler;
    char _pad[FQ_CACHE_LINE_SIZE];
} fq_worker_t;

struct fq_scheduler_t {
    fq_worker_t           *workers;
    unsigned               worker_count;
    fq_queue_t            *global_queue;
    fq_mutex_t             submit_mutex;
    fq_condition_t         idle_cond;
    fq_atomic_int_t        shutdown_flag;
    fq_atomic_int_t        idle_count;
    fq_atomic_int_t        tasks_submitted;
    fq_atomic_int_t        tasks_completed;
    fq_atomic_int_t        tasks_canceled;
    fq_atomic_int_t        tasks_pending;
    fq_atomic_int_t        tasks_active;
    fq_bool_t              enable_work_stealing;
    fq_bool_t              enable_priority;
    uint64_t               total_wait_ns;
    uint64_t               total_work_ns;
    const fq_allocator_t  *allocator;
    fq_log_fn              log_callback;
    void                  *log_context;
    fq_log_level_t         log_level;
};

 

static void log_msg(fq_scheduler_t *s, fq_log_level_t level, const char *msg)
{
    if (s->log_callback && level <= s->log_level)
        s->log_callback(level, msg, s->log_context);
}

 

static fq_bool_t try_steal(fq_scheduler_t *s, fq_worker_t *thief,
                            fq_task_t **out)
{
    for (unsigned i = 0; i < s->worker_count; ++i) {
        unsigned idx = (thief->id + 1 + i) % s->worker_count;
        if (idx == thief->id) continue;
        if (fq_queue_try_pop(s->workers[idx].queue, out) == FQ_OK)
            return FQ_TRUE;
    }
    return FQ_FALSE;
}

static int worker_main(void *arg)
{
    fq_worker_t *w = (fq_worker_t *)arg;
    fq_scheduler_t *s = w->scheduler;

    char buf[64];
    snprintf(buf, sizeof(buf), "worker %u started", w->id);
    log_msg(s, FQ_LOG_DEBUG, buf);

    unsigned idle_backoff = 0;

    for (;;) {
        if (fq_atomic_load_explicit(&s->shutdown_flag, FQ_MEMORY_ORDER_ACQUIRE) != 0 &&
            fq_queue_empty(s->global_queue) && fq_queue_empty(w->queue))
            break;

        fq_task_t *task = NULL;
        fq_bool_t found = FQ_FALSE;

        /* When priority is enabled, check global queue first. */
        if (s->enable_priority) {
            if (fq_queue_try_pop(s->global_queue, &task) == FQ_OK) {
                found = FQ_TRUE;
            } else if (fq_queue_try_pop(w->queue, &task) == FQ_OK) {
                found = FQ_TRUE;
            } else if (s->enable_work_stealing && try_steal(s, w, &task)) {
                found = FQ_TRUE;
            }
        } else {
            if (fq_queue_try_pop(w->queue, &task) == FQ_OK) {
                found = FQ_TRUE;
            } else if (s->enable_work_stealing && try_steal(s, w, &task)) {
                found = FQ_TRUE;
            } else if (fq_queue_try_pop(s->global_queue, &task) == FQ_OK) {
                found = FQ_TRUE;
            }
        }

        if (found && task) {
            idle_backoff = 0;
            fq_atomic_fetch_add_explicit(&s->tasks_active, 1,
                                         FQ_MEMORY_ORDER_RELEASE);
            fq_atomic_fetch_sub_explicit(&s->tasks_pending, 1,
                                         FQ_MEMORY_ORDER_RELEASE);

            int64_t start = fq_time_now_ns();
            fq_task_execute(task);
            int64_t end = fq_time_now_ns();

            fq_atomic_fetch_add_explicit(&s->tasks_completed, 1,
                                         FQ_MEMORY_ORDER_RELEASE);
            fq_atomic_fetch_sub_explicit(&s->tasks_active, 1,
                                         FQ_MEMORY_ORDER_RELEASE);

            /* Accumulate work time (unsigned wraparound is fine). */
            s->total_work_ns += (uint64_t)(end - start);

            fq_task_destroy(task);
        } else {
            fq_atomic_fetch_add_explicit(&s->idle_count, 1,
                                         FQ_MEMORY_ORDER_RELAXED);

            int64_t wait_start = fq_time_now_ns();

            fq_mutex_lock(&s->submit_mutex);
            if (fq_queue_empty(s->global_queue) && fq_queue_empty(w->queue)) {
                unsigned timeout = 1u << (idle_backoff < 4 ? idle_backoff : 4);
                if (timeout > 16) timeout = 16;
                fq_condition_timedwait(&s->idle_cond, &s->submit_mutex, timeout);
                if (idle_backoff < 6) idle_backoff++;
            }
            fq_mutex_unlock(&s->submit_mutex);

            int64_t wait_end = fq_time_now_ns();
            s->total_wait_ns += (uint64_t)(wait_end - wait_start);

            fq_atomic_fetch_sub_explicit(&s->idle_count, 1,
                                         FQ_MEMORY_ORDER_RELAXED);
        }
    }

    snprintf(buf, sizeof(buf), "worker %u stopping", w->id);
    log_msg(s, FQ_LOG_DEBUG, buf);
    return 0;
}

 

void fq_scheduler_config_default(fq_scheduler_config_t *config)
{
    if (!config) return;
    memset(config, 0, sizeof(*config));
    config->thread_count           = FQ_DEFAULT_THREADS;
    config->queue_capacity         = FQ_QUEUE_DEFAULT_CAPACITY;
    config->enable_work_stealing   = FQ_TRUE;
    config->enable_priority        = FQ_FALSE;
    config->allocator              = NULL;
    config->log_callback           = NULL;
    config->log_context            = NULL;
    config->log_level              = FQ_DEFAULT_LOG_LEVEL;
}

 

fq_status_t fq_scheduler_create(fq_scheduler_t **scheduler,
                                const fq_scheduler_config_t *config)
{
    if (!scheduler) return FQ_ERR_INVAL;

    fq_scheduler_config_t cfg;
    if (config) {
        cfg = *config;
    } else {
        fq_scheduler_config_default(&cfg);
    }

    const fq_allocator_t *alloc = cfg.allocator;
    if (!alloc) alloc = fq_default_allocator();

    fq_scheduler_t *s = fq_calloc(alloc, 1, sizeof(*s));
    if (!s) return FQ_ERR_NOMEM;

    s->allocator    = alloc;
    s->log_callback = cfg.log_callback;
    s->log_context  = cfg.log_context;
    s->log_level    = cfg.log_level;
    s->enable_work_stealing = cfg.enable_work_stealing;
    s->enable_priority      = cfg.enable_priority;
    s->total_wait_ns = 0;
    s->total_work_ns = 0;

    fq_atomic_store_explicit(&s->shutdown_flag, 0, FQ_MEMORY_ORDER_RELAXED);

    fq_status_t st = fq_queue_create(&s->global_queue, cfg.queue_capacity, alloc);
    if (st != FQ_OK) { fq_free(alloc, s); return st; }

    unsigned tc = cfg.thread_count;
    if (tc == 0) tc = fq_hardware_concurrency();
    if (tc == 0) tc = 1;
    if (tc > FQ_MAX_THREADS) tc = FQ_MAX_THREADS;
    s->worker_count = tc;

    if (fq_mutex_init(&s->submit_mutex) != 0) {
        fq_queue_destroy(s->global_queue);
        fq_free(alloc, s);
        return FQ_ERR_INTERNAL;
    }
    if (fq_condition_init(&s->idle_cond) != 0) {
        fq_mutex_destroy(&s->submit_mutex);
        fq_queue_destroy(s->global_queue);
        fq_free(alloc, s);
        return FQ_ERR_INTERNAL;
    }

    s->workers = fq_calloc(alloc, tc, sizeof(fq_worker_t));
    if (!s->workers) {
        fq_condition_destroy(&s->idle_cond);
        fq_mutex_destroy(&s->submit_mutex);
        fq_queue_destroy(s->global_queue);
        fq_free(alloc, s);
        return FQ_ERR_NOMEM;
    }

    for (unsigned i = 0; i < tc; ++i) {
        fq_worker_t *w = &s->workers[i];
        w->id         = i;
        w->scheduler  = s;
        w->running    = FQ_TRUE;

        st = fq_queue_create(&w->queue, cfg.queue_capacity / tc, alloc);
        if (st != FQ_OK) {
            for (unsigned j = 0; j < i; ++j) {
                fq_thread_join(&s->workers[j].thread, NULL);
                fq_queue_destroy(s->workers[j].queue);
            }
            fq_free(alloc, s->workers);
            fq_condition_destroy(&s->idle_cond);
            fq_mutex_destroy(&s->submit_mutex);
            fq_queue_destroy(s->global_queue);
            fq_free(alloc, s);
            return st;
        }

        if (fq_thread_create(&w->thread, worker_main, w) != 0) {
            fq_queue_destroy(w->queue);
            for (unsigned j = 0; j < i; ++j) {
                fq_thread_join(&s->workers[j].thread, NULL);
                fq_queue_destroy(s->workers[j].queue);
            }
            fq_free(alloc, s->workers);
            fq_condition_destroy(&s->idle_cond);
            fq_mutex_destroy(&s->submit_mutex);
            fq_queue_destroy(s->global_queue);
            fq_free(alloc, s);
            return FQ_ERR_INTERNAL;
        }
    }

    log_msg(s, FQ_LOG_INFO, "scheduler created");
    *scheduler = s;
    return FQ_OK;
}

fq_status_t fq_scheduler_shutdown(fq_scheduler_t *scheduler)
{
    if (!scheduler) return FQ_ERR_INVAL;

    log_msg(scheduler, FQ_LOG_INFO, "scheduler shutting down");
    fq_atomic_store_explicit(&scheduler->shutdown_flag, 1,
                             FQ_MEMORY_ORDER_RELEASE);

    fq_condition_broadcast(&scheduler->idle_cond);

    for (unsigned i = 0; i < scheduler->worker_count; ++i)
        fq_thread_join(&scheduler->workers[i].thread, NULL);

    for (unsigned i = 0; i < scheduler->worker_count; ++i)
        fq_queue_destroy(scheduler->workers[i].queue);

    fq_free(scheduler->allocator, scheduler->workers);
    fq_condition_destroy(&scheduler->idle_cond);
    fq_mutex_destroy(&scheduler->submit_mutex);
    fq_queue_destroy(scheduler->global_queue);
    fq_free(scheduler->allocator, scheduler);

    return FQ_OK;
}

void fq_scheduler_cancel_all(fq_scheduler_t *scheduler)
{
    if (!scheduler) return;

    fq_atomic_store_explicit(&scheduler->shutdown_flag, 1,
                             FQ_MEMORY_ORDER_RELEASE);

    /* Use a heap-allocated buffer to avoid VLA stack overflow. */
    enum { BATCH = 256 };
    fq_task_t *tasks[BATCH];
    size_t n;
    while ((n = fq_queue_drain(scheduler->global_queue, tasks, BATCH)) > 0) {
        for (size_t i = 0; i < n; ++i) {
            fq_task_destroy(tasks[i]);
            fq_atomic_fetch_add_explicit(&scheduler->tasks_canceled, 1,
                                         FQ_MEMORY_ORDER_RELAXED);
        }
    }

    for (unsigned i = 0; i < scheduler->worker_count; ++i) {
        n = fq_queue_drain(scheduler->workers[i].queue, tasks, BATCH);
        for (size_t j = 0; j < n; ++j) {
            fq_task_destroy(tasks[j]);
            fq_atomic_fetch_add_explicit(&scheduler->tasks_canceled, 1,
                                         FQ_MEMORY_ORDER_RELAXED);
        }
    }

    fq_condition_broadcast(&scheduler->idle_cond);
}

 

fq_status_t fq_scheduler_submit(fq_scheduler_t *scheduler, fq_task_t *task)
{
    if (!scheduler || !task) return FQ_ERR_INVAL;
    if (fq_atomic_load_explicit(&scheduler->shutdown_flag,
                                FQ_MEMORY_ORDER_ACQUIRE))
        return FQ_ERR_CLOSED;

    fq_atomic_fetch_add_explicit(&scheduler->tasks_submitted, 1,
                                 FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_fetch_add_explicit(&scheduler->tasks_pending, 1,
                                 FQ_MEMORY_ORDER_RELEASE);

    fq_status_t st;

    /* When priority is enabled, high/urgent tasks go to global queue. */
    if (scheduler->enable_priority &&
        fq_task_priority(task) >= FQ_PRIORITY_HIGH) {
        st = fq_queue_push(scheduler->global_queue, task);
    } else {
        static _Thread_local unsigned tls_next_worker = 0;
        unsigned idx = tls_next_worker % scheduler->worker_count;
        tls_next_worker++;

        st = fq_queue_push(scheduler->workers[idx].queue, task);
        if (st != FQ_OK) {
            st = fq_queue_push(scheduler->global_queue, task);
        }
    }

    if (st == FQ_OK) {
        fq_condition_signal(&scheduler->idle_cond);
    } else {
        fq_atomic_fetch_sub_explicit(&scheduler->tasks_pending, 1,
                                     FQ_MEMORY_ORDER_RELAXED);
    }

    return st;
}

fq_status_t fq_scheduler_submit_with_future(fq_scheduler_t *scheduler,
                                            fq_task_t *task,
                                            fq_future_t **future)
{
    if (!scheduler || !task) return FQ_ERR_INVAL;

    fq_future_t *f = NULL;
    fq_status_t st = fq_future_create(&f, scheduler->allocator);
    if (st != FQ_OK) return st;

    fq_task_set_future(task, f);

    st = fq_scheduler_submit(scheduler, task);
    if (st != FQ_OK) {
        fq_future_destroy(f);
        return st;
    }

    if (future) *future = f;
    return FQ_OK;
}

fq_status_t fq_scheduler_submit_fn(fq_scheduler_t *scheduler,
                                   fq_task_fn fn, void *user_data)
{
    if (!scheduler || !fn) return FQ_ERR_INVAL;

    fq_task_t *task = NULL;
    fq_status_t st = fq_task_create(&task, fn, user_data, scheduler->allocator);
    if (st != FQ_OK) return st;

    st = fq_scheduler_submit(scheduler, task);
    if (st != FQ_OK) fq_task_destroy(task);
    return st;
}

 

void fq_scheduler_wait_idle(fq_scheduler_t *scheduler)
{
    if (!scheduler) return;

    for (;;) {
        fq_bool_t idle = fq_scheduler_is_idle(scheduler);
        if (idle) break;
        fq_thread_yield();
        fq_thread_sleep_ms(0);
    }
}

fq_bool_t fq_scheduler_is_idle(const fq_scheduler_t *scheduler)
{
    if (!scheduler) return FQ_TRUE;

    /* Read all three counters.  If pending + active == 0 and the queue is
       empty, every submitted task has been completed or cancelled. */
    int pending = fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_pending,
        FQ_MEMORY_ORDER_ACQUIRE);
    int active = fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_active,
        FQ_MEMORY_ORDER_ACQUIRE);

    if (pending != 0 || active != 0) return FQ_FALSE;

    /* Second pass to close the TOCTOU window where a worker pops a task
       between our two reads. */
    pending = fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_pending,
        FQ_MEMORY_ORDER_ACQUIRE);
    active = fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_active,
        FQ_MEMORY_ORDER_ACQUIRE);

    return pending == 0 && active == 0;
}

fq_bool_t fq_scheduler_is_shutdown(const fq_scheduler_t *scheduler)
{
    if (!scheduler) return FQ_TRUE;
    return fq_atomic_load_explicit(
               &((fq_scheduler_t *)scheduler)->shutdown_flag,
               FQ_MEMORY_ORDER_ACQUIRE) != 0;
}

 

void fq_scheduler_stats(const fq_scheduler_t *scheduler,
                        fq_scheduler_stats_t *stats)
{
    if (!scheduler || !stats) return;
    memset(stats, 0, sizeof(*stats));
    stats->thread_count    = scheduler->worker_count;
    stats->tasks_submitted = (size_t)(unsigned)fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_submitted,
        FQ_MEMORY_ORDER_RELAXED);
    stats->tasks_completed = (size_t)(unsigned)fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_completed,
        FQ_MEMORY_ORDER_RELAXED);
    stats->tasks_canceled = (size_t)(unsigned)fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_canceled,
        FQ_MEMORY_ORDER_RELAXED);
    stats->tasks_pending = (size_t)(unsigned)fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_pending,
        FQ_MEMORY_ORDER_RELAXED);
    stats->tasks_active = (size_t)(unsigned)fq_atomic_load_explicit(
        &((fq_scheduler_t *)scheduler)->tasks_active,
        FQ_MEMORY_ORDER_RELAXED);
    stats->total_wait_ns = scheduler->total_wait_ns;
    stats->total_work_ns = scheduler->total_work_ns;
}
