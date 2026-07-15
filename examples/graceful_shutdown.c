/**
 * @file examples/graceful_shutdown.c
 * @brief Example: graceful shutdown of a scheduler.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static void long_task(void *arg)
{
    int id = *(int *)arg;
    printf("  Task %d started\n", id);
    fq_thread_sleep_ms(50);
    printf("  Task %d done\n", id);
}

int main(void)
{
    printf("FastQueue v%s — Graceful Shutdown Example\n\n",
           fq_version_string());

    fq_scheduler_t *s = NULL;
    fq_scheduler_config_default(NULL);

    if (fq_scheduler_create(&s, NULL) != FQ_OK) {
        fprintf(stderr, "Failed to create scheduler\n");
        return 1;
    }

    int ids[10];
    for (int i = 0; i < 10; ++i) {
        ids[i] = i;
        fq_scheduler_submit_fn(s, long_task, &ids[i]);
    }

    printf("Submitting done, shutting down gracefully...\n");
    fq_scheduler_shutdown(s);
    printf("Scheduler shut down.\n");
    return 0;
}
