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

    /* Submit 100 tasks. */
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

    /* Each task increments 1000 times. */
    if (fq_atomic_load(&g_stress_counter) != 10000 * 1000) {
        fq_scheduler_shutdown(s);
        return 3;
    }

    fq_scheduler_shutdown(s);
    return 0;
}
