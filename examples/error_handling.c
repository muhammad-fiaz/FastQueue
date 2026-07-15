/**
 * @file examples/error_handling.c
 * @brief Example: error handling patterns.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

static void might_fail(void *arg)
{
    int *counter = (int *)arg;
    (*counter)++;
    if (*counter == 5) {
        fq_set_error("deliberate error at task %d", *counter);
    }
}

int main(void)
{
    printf("FastQueue v%s — Error Handling Example\n\n",
           fq_version_string());

    /* Demonstrate error codes. */
    printf("Error codes:\n");
    printf("  FQ_OK            = %s\n", fq_status_string(FQ_OK));
    printf("  FQ_ERR_NOMEM     = %s\n", fq_status_string(FQ_ERR_NOMEM));
    printf("  FQ_ERR_INVAL     = %s\n", fq_status_string(FQ_ERR_INVAL));
    printf("  FQ_ERR_BUSY      = %s\n", fq_status_string(FQ_ERR_BUSY));
    printf("  FQ_ERR_TIMEOUT   = %s\n", fq_status_string(FQ_ERR_TIMEOUT));
    printf("  FQ_ERR_CANCELED  = %s\n", fq_status_string(FQ_ERR_CANCELED));
    printf("  FQ_ERR_OVERFLOW  = %s\n", fq_status_string(FQ_ERR_OVERFLOW));
    printf("  FQ_ERR_CLOSED    = %s\n", fq_status_string(FQ_ERR_CLOSED));

    /* Demonstrate parameter validation. */
    fq_status_t st;
    st = fq_scheduler_create(NULL, NULL);
    printf("\nNULL param: %s\n", fq_status_string(st));

    fq_task_t *t = NULL;
    st = fq_task_create(&t, NULL, NULL, NULL);
    printf("NULL callback: %s\n", fq_status_string(st));

    /* Demonstrate future timeout. */
    fq_future_t *f = NULL;
    fq_future_create(&f, NULL);
    fq_status_t fstatus;
    st = fq_future_wait_timeout(f, 10, &fstatus);
    printf("Future timeout: %s\n", fq_status_string(st));
    fq_future_destroy(f);

    printf("\nAll error patterns demonstrated.\n");
    return 0;
}
