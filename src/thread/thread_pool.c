/**
 * @file thread_pool.c
 * @brief High-level thread pool API.
 *
 * The thread pool is a convenience wrapper around fq_scheduler_t.
 */

#include "fastqueue/thread_pool.h"
#include "fastqueue/memory.h"

/* ── A thread pool is literally a scheduler ──────────────────────────── */

fq_status_t fq_thread_pool_create(fq_thread_pool_t **pool)
{
    return fq_scheduler_create((fq_scheduler_t **)pool, NULL);
}

fq_status_t fq_thread_pool_create_ex(fq_thread_pool_t **pool,
                                     unsigned thread_count)
{
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = thread_count;
    return fq_scheduler_create((fq_scheduler_t **)pool, &cfg);
}

fq_status_t fq_thread_pool_create_configured(
    fq_thread_pool_t **pool,
    const fq_scheduler_config_t *config)
{
    return fq_scheduler_create((fq_scheduler_t **)pool, config);
}

fq_status_t fq_thread_pool_shutdown(fq_thread_pool_t *pool)
{
    return fq_scheduler_shutdown((fq_scheduler_t *)pool);
}

fq_status_t fq_thread_pool_submit(fq_thread_pool_t *pool, fq_task_t *task)
{
    return fq_scheduler_submit((fq_scheduler_t *)pool, task);
}

fq_status_t fq_thread_pool_submit_fn(fq_thread_pool_t *pool,
                                     fq_task_fn fn, void *user_data)
{
    return fq_scheduler_submit_fn((fq_scheduler_t *)pool, fn, user_data);
}

fq_status_t fq_thread_pool_submit_with_future(fq_thread_pool_t *pool,
                                              fq_task_t *task,
                                              fq_future_t **future)
{
    return fq_scheduler_submit_with_future((fq_scheduler_t *)pool,
                                           task, future);
}

void fq_thread_pool_wait_idle(fq_thread_pool_t *pool)
{
    fq_scheduler_wait_idle((fq_scheduler_t *)pool);
}

fq_bool_t fq_thread_pool_is_idle(const fq_thread_pool_t *pool)
{
    return fq_scheduler_is_idle((const fq_scheduler_t *)pool);
}

unsigned fq_thread_pool_size(const fq_thread_pool_t *pool)
{
    if (!pool) return 0;
    fq_scheduler_stats_t stats;
    fq_scheduler_stats((const fq_scheduler_t *)pool, &stats);
    return stats.thread_count;
}

void fq_thread_pool_stats(const fq_thread_pool_t *pool,
                          fq_scheduler_stats_t *stats)
{
    fq_scheduler_stats((const fq_scheduler_t *)pool, stats);
}
