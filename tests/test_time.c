/**
 * @file test_time.c
 * @brief Time utility tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

int test_time_now_ns(void)
{
    int64_t t1 = fq_time_now_ns();
    if (t1 <= 0) return 1;

    fq_thread_sleep_ms(10);

    int64_t t2 = fq_time_now_ns();
    if (t2 <= t1) return 2;

    return 0;
}

int test_timer(void)
{
    fq_timer_t timer;
    fq_timer_start(&timer);

    fq_thread_sleep_ms(20);

    int64_t elapsed_ns = fq_timer_elapsed_ns(&timer);
    if (elapsed_ns < 10000000) return 1; /* at least 10ms */

    int64_t elapsed_ms = fq_timer_elapsed_ms(&timer);
    if (elapsed_ms < 10) return 2;

    return 0;
}
