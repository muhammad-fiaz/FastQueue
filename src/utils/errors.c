/**
 * @file errors.c
 * @brief Error handling utilities.
 */

#include "fastqueue/errors.h"

#include <stdio.h>
#include <string.h>

#ifdef FQ_OS_WINDOWS
#    define FQ_THREAD_LOCAL __declspec(thread)
#else
#    define FQ_THREAD_LOCAL _Thread_local
#endif

static FQ_THREAD_LOCAL char tls_error[512] = {0};

const char *fq_status_string(fq_status_t status)
{
    switch (status) {
    case FQ_OK:            return "success";
    case FQ_ERR_NOMEM:     return "out of memory";
    case FQ_ERR_INVAL:     return "invalid argument";
    case FQ_ERR_BUSY:      return "resource busy";
    case FQ_ERR_TIMEOUT:   return "timed out";
    case FQ_ERR_CANCELED:  return "canceled";
    case FQ_ERR_OVERFLOW:  return "queue full";
    case FQ_ERR_CLOSED:    return "system shut down";
    case FQ_ERR_INTERNAL:  return "internal error";
    case FQ_ERR_NOSUPPORT: return "not supported";
    }
    return "unknown error";
}

fq_bool_t fq_status_ok(fq_status_t status)
{
    return status == FQ_OK ? FQ_TRUE : FQ_FALSE;
}

const char *fq_last_error(void)
{
    return tls_error;
}

void fq_set_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf(tls_error, sizeof(tls_error), fmt, args);
    va_end(args);
}
