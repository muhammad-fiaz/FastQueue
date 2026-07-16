/**
 * @file test_platform.c
 * @brief Platform abstraction tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

int test_hardware_concurrency(void)
{
    unsigned cores = fq_hardware_concurrency();
    /* Should be at least 1 on any system. */
    if (cores == 0) return 1;
    return 0;
}

int test_thread_sleep(void)
{
    int64_t before = fq_time_now_ns();
    fq_thread_sleep_ms(50);
    int64_t after = fq_time_now_ns();

    int64_t diff_ms = (after - before) / 1000000;
    /* Should be at least 40ms (allowing for scheduling imprecision). */
    if (diff_ms < 40) return 1;
    return 0;
}

int test_thread_yield(void)
{
    /* Just verify it doesn't crash. */
    fq_thread_yield();
    return 0;
}
