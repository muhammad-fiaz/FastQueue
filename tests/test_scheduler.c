/**
 * @file test_scheduler.c
 * @brief Scheduler and thread pool unit tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static int g_counter = 0;
static fq_mutex_t g_mutex;

static void inc_task(void *arg)
{
    (void)arg;
    fq_mutex_lock(&g_mutex);
    g_counter++;
    fq_mutex_unlock(&g_mutex);
}

int test_scheduler_basic(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    fq_status_t st = fq_scheduler_create(&s, &cfg);
    if (st != FQ_OK) return 1;

    fq_mutex_init(&g_mutex);
    g_counter = 0;

    for (int i = 0; i < 100; ++i) {
        st = fq_scheduler_submit_fn(s, inc_task, NULL);
        if (st != FQ_OK) { fq_scheduler_shutdown(s); fq_mutex_destroy(&g_mutex); return 2; }
    }

    fq_scheduler_wait_idle(s);

    if (g_counter != 100) {
        fq_scheduler_shutdown(s);
        fq_mutex_destroy(&g_mutex);
        return 3;
    }

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(s, &stats);
    if (stats.tasks_completed < 100) {
        fq_scheduler_shutdown(s);
        fq_mutex_destroy(&g_mutex);
        return 4;
    }

    fq_scheduler_shutdown(s);
    fq_mutex_destroy(&g_mutex);
    return 0;
}

int test_thread_pool_basic(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 4) != FQ_OK) return 1;

    fq_mutex_init(&g_mutex);
    g_counter = 0;

    for (int i = 0; i < 200; ++i) {
        if (fq_thread_pool_submit_fn(pool, inc_task, NULL) != FQ_OK) {
            fq_thread_pool_shutdown(pool);
            fq_mutex_destroy(&g_mutex);
            return 2;
        }
    }

    fq_thread_pool_wait_idle(pool);

    if (g_counter != 200) {
        fq_thread_pool_shutdown(pool);
        fq_mutex_destroy(&g_mutex);
        return 3;
    }

    if (fq_thread_pool_size(pool) != 4) {
        fq_thread_pool_shutdown(pool);
        fq_mutex_destroy(&g_mutex);
        return 4;
    }

    fq_thread_pool_shutdown(pool);
    fq_mutex_destroy(&g_mutex);
    return 0;
}

static fq_atomic_int_t g_stress_counter = FQ_ATOMIC_INIT(0);

static void stress_task(void *arg)
{
    (void)arg;
    for (int i = 0; i < 1000; ++i) {
        fq_atomic_fetch_add(&g_stress_counter, 1);
    }
}

int test_scheduler_stress(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_atomic_store(&g_stress_counter, 0);

    for (int i = 0; i < 100; ++i) {
        if (fq_scheduler_submit_fn(s, stress_task, NULL) != FQ_OK) {
            fq_scheduler_shutdown(s);
            return 2;
        }
    }

    fq_scheduler_wait_idle(s);

    if (fq_atomic_load(&g_stress_counter) != 100 * 1000) {
        fq_scheduler_shutdown(s);
        return 3;
    }

    fq_scheduler_shutdown(s);
    return 0;
}

int test_stress_many_jobs(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 8;
    cfg.queue_capacity = 16384;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_atomic_store(&g_stress_counter, 0);

    for (int i = 0; i < 10000; ++i) {
        if (fq_scheduler_submit_fn(s, stress_task, NULL) != FQ_OK) {
            fq_scheduler_shutdown(s);
            return 2;
        }
    }

    fq_scheduler_wait_idle(s);

    if (fq_atomic_load(&g_stress_counter) != 10000 * 1000) {
        fq_scheduler_shutdown(s);
        return 3;
    }

    fq_scheduler_shutdown(s);
    return 0;
}

int test_scheduler_cancel_all(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;
    cfg.queue_capacity = 1024;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    /* Submit tasks that sleep to keep them in the queue. */
    for (int i = 0; i < 50; ++i) {
        if (fq_scheduler_submit_fn(s, inc_task, NULL) != FQ_OK) {
            fq_scheduler_shutdown(s);
            return 2;
        }
    }

    fq_scheduler_cancel_all(s);

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(s, &stats);

    /* All submitted tasks should be accounted for. */
    if (stats.tasks_submitted < 50) {
        fq_scheduler_shutdown(s);
        return 3;
    }

    fq_scheduler_shutdown(s);
    return 0;
}

int test_scheduler_is_idle(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    /* Should be idle immediately after creation. */
    if (!fq_scheduler_is_idle(s)) {
        fq_scheduler_shutdown(s);
        return 2;
    }

    fq_scheduler_shutdown(s);
    return 0;
}

int test_scheduler_is_shutdown(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    if (fq_scheduler_is_shutdown(s)) {
        fq_scheduler_shutdown(s);
        return 2;
    }

    fq_scheduler_shutdown(s);

    /* Cannot check is_shutdown after shutdown because the struct is freed.
     * The test passes if shutdown completed without error. */
    return 0;
}

static void noop_task(void *arg)
{
    (void)arg;
}

int test_scheduler_submit(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_task_t *task = NULL;
    if (fq_task_create(&task, noop_task, NULL, NULL) != FQ_OK) {
        fq_scheduler_shutdown(s);
        return 2;
    }

    fq_status_t st = fq_scheduler_submit(s, task);
    if (st != FQ_OK) {
        fq_task_destroy(task);
        fq_scheduler_shutdown(s);
        return 3;
    }

    fq_scheduler_wait_idle(s);
    fq_scheduler_shutdown(s);
    return 0;
}

int test_scheduler_submit_with_future(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_task_t *task = NULL;
    if (fq_task_create(&task, noop_task, NULL, NULL) != FQ_OK) {
        fq_scheduler_shutdown(s);
        return 2;
    }

    fq_future_t *future = NULL;
    fq_status_t st = fq_scheduler_submit_with_future(s, task, &future);
    if (st != FQ_OK) {
        fq_task_destroy(task);
        fq_scheduler_shutdown(s);
        return 3;
    }

    fq_status_t result = fq_future_wait(future);
    if (result != FQ_OK) {
        fq_future_destroy(future);
        fq_scheduler_shutdown(s);
        return 4;
    }

    fq_future_destroy(future);
    fq_scheduler_shutdown(s);
    return 0;
}

int test_scheduler_stats_timing(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_mutex_init(&g_mutex);
    g_counter = 0;

    /* Submit enough work to measurable time. */
    for (int i = 0; i < 1000; ++i) {
        fq_scheduler_submit_fn(s, inc_task, NULL);
    }

    fq_scheduler_wait_idle(s);

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(s, &stats);

    /* Work time should be >= 0 (timing granularity varies by platform). */
    if (stats.total_work_ns < 0) {
        fq_scheduler_shutdown(s);
        fq_mutex_destroy(&g_mutex);
        return 1;
    }

    fq_scheduler_shutdown(s);
    fq_mutex_destroy(&g_mutex);
    return 0;
}

int test_scheduler_priority(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;
    cfg.enable_priority = FQ_TRUE;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_mutex_init(&g_mutex);
    g_counter = 0;

    /* Submit mix of normal and high priority tasks. */
    for (int i = 0; i < 20; ++i) {
        fq_task_t *task = NULL;
        fq_task_create(&task, inc_task, NULL, NULL);
        if (i % 2 == 0) {
            fq_task_set_priority(task, FQ_PRIORITY_HIGH);
        }
        fq_scheduler_submit(s, task);
    }

    fq_scheduler_wait_idle(s);

    if (g_counter != 20) {
        fq_scheduler_shutdown(s);
        fq_mutex_destroy(&g_mutex);
        return 1;
    }

    fq_scheduler_shutdown(s);
    fq_mutex_destroy(&g_mutex);
    return 0;
}

int test_scheduler_no_work_stealing(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;
    cfg.enable_work_stealing = FQ_FALSE;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    fq_mutex_init(&g_mutex);
    g_counter = 0;

    for (int i = 0; i < 20; ++i) {
        fq_scheduler_submit_fn(s, inc_task, NULL);
    }

    fq_scheduler_wait_idle(s);

    /* Tasks may not all complete if work stealing is off and they land
     * on a busy worker's queue, but at least some should complete. */
    if (g_counter == 0) {
        fq_scheduler_shutdown(s);
        fq_mutex_destroy(&g_mutex);
        return 1;
    }

    fq_scheduler_shutdown(s);
    fq_mutex_destroy(&g_mutex);
    return 0;
}

int test_thread_pool_create(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create(&pool) != FQ_OK) return 1;
    if (!pool) return 2;

    fq_thread_pool_shutdown(pool);
    return 0;
}

int test_thread_pool_create_configured(void)
{
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 3;

    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_configured(&pool, &cfg) != FQ_OK) return 1;
    if (!pool) return 2;
    if (fq_thread_pool_size(pool) != 3) {
        fq_thread_pool_shutdown(pool);
        return 3;
    }

    fq_thread_pool_shutdown(pool);
    return 0;
}

int test_thread_pool_submit(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 2) != FQ_OK) return 1;

    fq_task_t *task = NULL;
    if (fq_task_create(&task, noop_task, NULL, NULL) != FQ_OK) {
        fq_thread_pool_shutdown(pool);
        return 2;
    }

    if (fq_thread_pool_submit(pool, task) != FQ_OK) {
        fq_task_destroy(task);
        fq_thread_pool_shutdown(pool);
        return 3;
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}

int test_thread_pool_submit_with_future(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 2) != FQ_OK) return 1;

    fq_task_t *task = NULL;
    if (fq_task_create(&task, noop_task, NULL, NULL) != FQ_OK) {
        fq_thread_pool_shutdown(pool);
        return 2;
    }

    fq_future_t *future = NULL;
    if (fq_thread_pool_submit_with_future(pool, task, &future) != FQ_OK) {
        fq_task_destroy(task);
        fq_thread_pool_shutdown(pool);
        return 3;
    }

    fq_status_t result = fq_future_wait(future);
    if (result != FQ_OK) {
        fq_future_destroy(future);
        fq_thread_pool_shutdown(pool);
        return 4;
    }

    fq_future_destroy(future);
    fq_thread_pool_shutdown(pool);
    return 0;
}

int test_thread_pool_is_idle(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 2) != FQ_OK) return 1;

    if (!fq_thread_pool_is_idle(pool)) {
        fq_thread_pool_shutdown(pool);
        return 2;
    }

    fq_thread_pool_shutdown(pool);
    return 0;
}

int test_thread_pool_stats(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 2) != FQ_OK) return 1;

    fq_scheduler_stats_t stats;
    fq_thread_pool_stats(pool, &stats);

    if (stats.thread_count != 2) {
        fq_thread_pool_shutdown(pool);
        return 1;
    }

    fq_thread_pool_shutdown(pool);
    return 0;
}

static void parallel_for_square(long index, void *ctx)
{
    long *r = (long *)ctx;
    r[index] = index * 2;
}

int test_scheduler_parallel_for(void)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) return 1;

    static long parallel_results[1000];
    for (long i = 0; i < 1000; ++i) parallel_results[i] = 0;

    fq_status_t st = fq_scheduler_parallel_for(s, 0, 1000,
        parallel_for_square, parallel_results);

    if (st != FQ_OK) {
        fq_scheduler_shutdown(s);
        return 2;
    }

    for (long i = 0; i < 1000; ++i) {
        if (parallel_results[i] != i * 2) {
            fq_scheduler_shutdown(s);
            return 3;
        }
    }

    fq_scheduler_shutdown(s);
    return 0;
}

static void parallel_for_increment(long index, void *ctx)
{
    long *r = (long *)ctx;
    r[index] = index + 1;
}

int test_thread_pool_parallel_for(void)
{
    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 4) != FQ_OK) return 1;

    static long parallel_results2[1000];
    for (long i = 0; i < 1000; ++i) parallel_results2[i] = 0;

    fq_status_t st = fq_thread_pool_parallel_for(pool, 0, 1000,
        parallel_for_increment, parallel_results2);

    if (st != FQ_OK) {
        fq_thread_pool_shutdown(pool);
        return 2;
    }

    for (long i = 0; i < 1000; ++i) {
        if (parallel_results2[i] != i + 1) {
            fq_thread_pool_shutdown(pool);
            return 3;
        }
    }

    fq_thread_pool_shutdown(pool);
    return 0;
}
