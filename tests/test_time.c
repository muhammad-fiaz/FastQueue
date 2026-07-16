/**
 * @file test_time.c
 * @brief Time utility tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

int test_time_now_ns(void)
{
    int64_t t1 = fq_time_now_ns();

    fq_thread_sleep_ms(50);

    int64_t t2 = fq_time_now_ns();

    /* Verify time advanced by at least 40ms. */
    int64_t diff_ms = (t2 - t1) / 1000000;
    if (diff_ms < 40) return 1;

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
