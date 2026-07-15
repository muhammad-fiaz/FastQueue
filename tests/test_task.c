/**
 * @file test_task.c
 * @brief Task unit tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static int g_task_executed = 0;

extern void fq_task_execute(fq_task_t *task);

static void noop_task(void *arg)
{
    (void)arg;
    g_task_executed = 1;
}

int test_task_create_destroy(void)
{
    fq_task_t *t = NULL;
    fq_status_t st = fq_task_create(&t, noop_task, NULL, NULL);
    if (st != FQ_OK) return 1;
    if (t == NULL) return 2;

    if (fq_task_user_data(t) != NULL) return 3;
    if (fq_task_priority(t) != FQ_PRIORITY_NORMAL) return 4;
    if (fq_task_is_executed(t)) return 5;
    if (fq_task_is_canceled(t)) return 6;

    fq_task_set_priority(t, FQ_PRIORITY_HIGH);
    if (fq_task_priority(t) != FQ_PRIORITY_HIGH) return 7;

    fq_task_destroy(t);
    return 0;
}

int test_task_execution(void)
{
    g_task_executed = 0;

    fq_task_t *t = NULL;
    if (fq_task_create(&t, noop_task, NULL, NULL) != FQ_OK) return 1;

    /* Execute manually via internal function. */
    fq_task_execute(t);

    if (!g_task_executed) return 2;
    if (!fq_task_is_executed(t)) return 3;

    fq_task_destroy(t);
    return 0;
}
