/**
 * @file examples/basic_jobs.c
 * @brief Example: submitting basic jobs to a thread pool.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static void print_task(void *arg)
{
    int id = *(int *)arg;
    printf("  Task %d executing on thread\n", id);
}

int main(void)
{
    printf("FastQueue v%s — Basic Jobs Example\n\n", fq_version_string());

    fq_thread_pool_t *pool = NULL;
    if (fq_thread_pool_create_ex(&pool, 4) != FQ_OK) {
        fprintf(stderr, "Failed to create pool\n");
        return 1;
    }

    int task_ids[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    printf("Submitting 8 tasks...\n");
    for (int i = 0; i < 8; ++i) {
        fq_thread_pool_submit_fn(pool, print_task, &task_ids[i]);
    }

    printf("Waiting for all tasks...\n");
    fq_thread_pool_wait_idle(pool);
    printf("Done.\n");

    fq_thread_pool_shutdown(pool);
    return 0;
}
