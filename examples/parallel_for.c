/**
 * @file examples/parallel_for.c
 * @brief Example: parallel loop over a range using fq_thread_pool_parallel_for.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

#define N 1000000

static double g_array[N];

static void square_index(long index, void *ctx)
{
    (void)ctx;
    g_array[index] = g_array[index] * g_array[index];
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

    /* Use the built-in parallel_for API. */
    fq_status_t st = fq_thread_pool_parallel_for(pool, 0, N, square_index, NULL);
    if (st != FQ_OK) {
        fprintf(stderr, "parallel_for failed: %s\n", fq_status_string(st));
        fq_thread_pool_shutdown(pool);
        return 1;
    }

    printf("g_array[0]      = %f\n", g_array[0]);
    printf("g_array[999999]  = %f\n", g_array[999999]);
    printf("Expected[0]     = %f\n", 0.0 * 0.0);
    printf("Expected[999999]= %f\n", 999999.0 * 999999.0);

    fq_thread_pool_shutdown(pool);
    return 0;
}
