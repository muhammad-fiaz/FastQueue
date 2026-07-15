#include "fastqueue/fastqueue.h"

#include <stdio.h>

#ifdef FQ_OS_WINDOWS
#    include <windows.h>
static uint64_t now_ns(void)
{
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (uint64_t)(cnt.QuadPart * 1000000000ULL / freq.QuadPart);
}
#else
#    include <time.h>
static uint64_t now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}
#endif

static fq_atomic_int_t g_sink = FQ_ATOMIC_INIT(0);

static void nop_bench(void *arg)
{
    (void)arg;
    fq_atomic_fetch_add_explicit(&g_sink, 1, FQ_MEMORY_ORDER_RELAXED);
}

static void bench_throughput(const char *label, unsigned threads, int total)
{
    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = threads;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) {
        fprintf(stderr, "  [ERROR] %s: failed to create scheduler\n", label);
        return;
    }

    for (int i = 0; i < 1000; ++i)
        fq_scheduler_submit_fn(s, nop_bench, NULL);
    fq_scheduler_wait_idle(s);
    g_sink = 0;

    uint64_t start = now_ns();

    for (int i = 0; i < total; ++i)
        fq_scheduler_submit_fn(s, nop_bench, NULL);
    fq_scheduler_wait_idle(s);

    uint64_t end = now_ns();

    double elapsed_ms = (double)(end - start) / 1000000.0;
    double throughput = (double)total / (elapsed_ms / 1000.0);

    printf("  %-25s  %2u threads   %10d jobs   %8.1f ms   %12.0f jobs/s\n",
           label, threads, total, elapsed_ms, throughput);

    fq_scheduler_shutdown(s);
}

int main(void)
{
    printf("FastQueue v%s - Throughput Benchmark\n", fq_version_string());
    printf("==============================================\n\n");

    bench_throughput("1 thread,  10K jobs",  1,  10000);
    bench_throughput("2 threads, 10K jobs",  2,  10000);
    bench_throughput("4 threads, 10K jobs",  4,  10000);
    bench_throughput("8 threads, 10K jobs",  8,  10000);
    printf("\n");
    bench_throughput("4 threads, 100K jobs", 4,  100000);
    bench_throughput("4 threads, 1M jobs",   4,  1000000);
    printf("\n");

    printf("Done.\n");
    return 0;
}
