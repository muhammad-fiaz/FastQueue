/**
 * @file examples/parallel_for.c
 * @brief Example: parallel loop over a range.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

#define N 1000000

static double g_array[N];

static void square_range(void *arg)
{
    uintptr_t range_id = (uintptr_t)arg;
    size_t start = (range_id * N) / 4;
    size_t end   = ((range_id + 1) * N) / 4;
    for (size_t i = start; i < end; ++i) {
        g_array[i] = g_array[i] * g_array[i];
    }
}

int main(void)
{
    printf("FastQueue v%s — Parallel For Example\n\n",
           fq_version_string());

    for (size_t i = 0; i < N; ++i) g_array[i] = (double)i;

    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 4) != FQ_OK) {
        fprintf(stderr, "Failed to create pool\n");
        return 1;
    }

    /* Split into 4 chunks. */
    for (int i = 0; i < 4; ++i) {
        fq_thread_pool_submit_fn(pool, square_range, (void *)(uintptr_t)i);
    }

    fq_thread_pool_wait_idle(pool);

    printf("g_array[0]     = %f\n", g_array[0]);
    printf("g_array[999999] = %f\n", g_array[999999]);

    fq_thread_pool_shutdown(pool);
    return 0;
}
