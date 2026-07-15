/**
 * @file test_errors.c
 * @brief Error handling tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>
#include <string.h>

int test_errors(void)
{
    /* Status strings. */
    const char *s = fq_status_string(FQ_OK);
    if (!s || strcmp(s, "success") != 0) return 1;

    s = fq_status_string(FQ_ERR_NOMEM);
    if (!s || strcmp(s, "out of memory") != 0) return 2;

    s = fq_status_string(FQ_ERR_INVAL);
    if (!s || strcmp(s, "invalid argument") != 0) return 3;

    s = fq_status_string(FQ_ERR_BUSY);
    if (!s || strcmp(s, "resource busy") != 0) return 4;

    s = fq_status_string(FQ_ERR_TIMEOUT);
    if (!s || strcmp(s, "timed out") != 0) return 5;

    s = fq_status_string(FQ_ERR_CANCELED);
    if (!s || strcmp(s, "canceled") != 0) return 6;

    s = fq_status_string(FQ_ERR_OVERFLOW);
    if (!s || strcmp(s, "queue full") != 0) return 7;

    s = fq_status_string(FQ_ERR_CLOSED);
    if (!s || strcmp(s, "system shut down") != 0) return 8;

    s = fq_status_string(FQ_ERR_INTERNAL);
    if (!s || strcmp(s, "internal error") != 0) return 9;

    /* fq_status_ok. */
    if (!fq_status_ok(FQ_OK)) return 10;
    if (fq_status_ok(FQ_ERR_NOMEM)) return 11;

    /* Thread-local error. */
    fq_set_error("test error %d", 42);
    const char *err = fq_last_error();
    if (!err) return 12;
    if (strstr(err, "42") == NULL) return 13;

    return 0;
}
