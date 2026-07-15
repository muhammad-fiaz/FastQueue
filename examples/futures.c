/**
 * @file examples/futures.c
 * @brief Example: using futures to obtain results from async tasks.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static void compute_square(void *arg)
{
    int *val = (int *)arg;
    *val = (*val) * (*val);
}

int main(void)
{
    printf("FastQueue v%s — Futures Example\n\n", fq_version_string());

    fq_scheduler_t *s = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    if (fq_scheduler_create(&s, &cfg) != FQ_OK) {
        fprintf(stderr, "Failed to create scheduler\n");
        return 1;
    }

    /* Submit a task with a future. */
    int value = 7;
    fq_task_t *task = NULL;
    fq_task_create(&task, compute_square, &value, NULL);

    fq_future_t *future = NULL;
    fq_scheduler_submit_with_future(s, task, &future);

    printf("Waiting for result...\n");
    fq_status_t st = fq_future_wait(future);
    printf("Status: %s\n", fq_status_string(st));
    printf("7^2 = %d\n", value);

    fq_future_destroy(future);
    fq_scheduler_shutdown(s);
    return 0;
}
