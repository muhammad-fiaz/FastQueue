/**
 * @file time.h
 * @brief Time utilities for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_TIME_H
#define FQ_TIME_H

#include "types.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct fq_timer_t
 * @brief High-resolution timer for measuring elapsed time.
 */
typedef struct fq_timer_t {
    int64_t start_ns;  /**< Start time in nanoseconds. */
} fq_timer_t;

/**
 * @brief Get the current time in nanoseconds (monotonic clock).
 *
 * @return Current time in nanoseconds since an arbitrary epoch.
 */
FQ_API int64_t fq_time_now_ns(void);

/**
 * @brief Initialize a timer and record the current time.
 *
 * @param timer Timer to initialize.
 */
FQ_API void fq_timer_start(fq_timer_t *timer);

/**
 * @brief Get the elapsed time since the timer was started.
 *
 * @param timer Previously started timer.
 *
 * @return Elapsed time in nanoseconds.
 */
FQ_API int64_t fq_timer_elapsed_ns(const fq_timer_t *timer);

/**
 * @brief Get the elapsed time since the timer was started, in milliseconds.
 *
 * @param timer Previously started timer.
 *
 * @return Elapsed time in milliseconds (may be fractional truncated to int).
 */
FQ_API int64_t fq_timer_elapsed_ms(const fq_timer_t *timer);

#ifdef __cplusplus
}
#endif

#endif /* FQ_TIME_H */
