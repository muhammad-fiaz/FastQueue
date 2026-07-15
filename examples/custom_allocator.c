/**
 * @file examples/custom_allocator.c
 * @brief Example: using a custom allocator.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>
#include <stdlib.h>

static size_t g_total = 0;

static void *my_alloc(size_t size, void *ctx)
{
    (void)ctx;
    g_total += size;
    return malloc(size);
}

static void *my_realloc(void *ptr, size_t size, void *ctx)
{
    (void)ctx;
    return realloc(ptr, size);
}

static void my_free(void *ptr, void *ctx)
{
    (void)ctx;
    free(ptr);
}

static void noop(void *arg) { (void)arg; }

int main(void)
{
    printf("FastQueue v%s — Custom Allocator Example\n\n",
           fq_version_string());

    fq_allocator_t alloc = {
        .alloc   = my_alloc,
        .realloc = my_realloc,
        .free    = my_free,
        .ctx     = NULL,
    };

    fq_thread_pool_t *pool = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;
    cfg.allocator = &alloc;

    if (fq_thread_pool_create_configured(&pool, &cfg) != FQ_OK) {
        fprintf(stderr, "Failed to create pool\n");
        return 1;
    }

    for (int i = 0; i < 50; ++i) {
        fq_thread_pool_submit_fn(pool, noop, NULL);
    }

    fq_thread_pool_wait_idle(pool);
    printf("Total bytes allocated via custom allocator: %zu\n", g_total);

    fq_thread_pool_shutdown(pool);
    return 0;
}
