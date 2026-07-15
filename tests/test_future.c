/**
 * @file test_future.c
 * @brief Future unit tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static void set_flag_task(void *arg)
{
    int *flag = (int *)arg;
    *flag = 1;
}

int test_future_wait(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    if (fq_future_is_ready(f)) return 2;

    /* Simulate fulfillment. */
    fq_future_set_result(f, FQ_OK);

    if (!fq_future_is_ready(f)) return 3;
    if (fq_future_status(f) != FQ_OK) return 4;

    fq_future_destroy(f);
    return 0;
}

int test_future_timeout(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    fq_status_t status;
    fq_status_t st = fq_future_wait_timeout(f, 10, &status);
    if (st != FQ_ERR_TIMEOUT) { fq_future_destroy(f); return 2; }

    fq_future_destroy(f);
    return 0;
}

int test_future_cancel(void)
{
    fq_future_t *f = NULL;
    if (fq_future_create(&f, NULL) != FQ_OK) return 1;

    fq_status_t st = fq_future_cancel(f);
    if (st != FQ_OK) return 2;
    if (!fq_future_is_canceled(f)) return 3;
    if (!fq_future_is_ready(f)) return 4;

    fq_future_destroy(f);
    return 0;
}
