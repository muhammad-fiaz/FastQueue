/**
 * @file errors.h
 * @brief Error handling utilities for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_ERRORS_H
#define FQ_ERRORS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Return a human-readable string for a status code.
 *
 * @param status Status code to describe.
 * @return Pointer to a static string such as "out of memory".
 */
const char *fq_status_string(fq_status_t status);

/**
 * @brief Check whether a status code indicates success.
 *
 * @param status Status code.
 * @return ::FQ_TRUE if @p status is ::FQ_OK, ::FQ_FALSE otherwise.
 */
fq_bool_t fq_status_ok(fq_status_t status);

/**
 * @brief Thread-local storage for the last error message.
 *
 * The returned pointer is valid until the next call on the same thread.
 *
 * @return Null-terminated error string, or an empty string if no error.
 */
const char *fq_last_error(void);

/**
 * @brief Set the thread-local error message.
 *
 * @param fmt printf-style format string.
 * @param ... Format arguments.
 */
void fq_set_error(const char *fmt, ...) FQ_PRINTF_FMT(1, 2);

#ifdef __cplusplus
}
#endif

#endif /* FQ_ERRORS_H */
