/**
 * @file time.c
 * @brief Time utilities implementation.
 */

#include "fastqueue/time.h"
#include "fastqueue/platform.h"

#if defined(FQ_OS_WINDOWS)
#include <windows.h>

static int64_t time_now_ns(void)
{
    static LARGE_INTEGER freq = {0};
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (int64_t)((counter.QuadPart * (int64_t)1000000000) / freq.QuadPart);
}

#elif defined(FQ_OS_POSIX)
#include <time.h>

static int64_t time_now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec;
}

#else
#include <time.h>

static int64_t time_now_ns(void)
{
    return (int64_t)clock() * (1000000000LL / CLOCKS_PER_SEC);
}

#endif

int64_t fq_time_now_ns(void)
{
    return time_now_ns();
}

void fq_timer_start(fq_timer_t *timer)
{
    if (timer) timer->start_ns = time_now_ns();
}

int64_t fq_timer_elapsed_ns(const fq_timer_t *timer)
{
    if (!timer) return 0;
    return time_now_ns() - timer->start_ns;
}

int64_t fq_timer_elapsed_ms(const fq_timer_t *timer)
{
    return fq_timer_elapsed_ns(timer) / 1000000;
}
