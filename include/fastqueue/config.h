/**
 * @file config.h
 * @brief Compile-time configuration options for FastQueue.
 *
 * All options can be overridden via compiler defines (-D) before including
 * any FastQueue header, or by editing this file directly.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_CONFIG_H
#define FQ_CONFIG_H

/**
 * @def FQ_MAX_THREADS
 * @brief Maximum number of worker threads the pool can create.
 */
#if !defined(FQ_MAX_THREADS)
#    define FQ_MAX_THREADS 256
#endif

/**
 * @def FQ_DEFAULT_THREADS
 * @brief Default number of worker threads (0 = hardware concurrency).
 */
#if !defined(FQ_DEFAULT_THREADS)
#    define FQ_DEFAULT_THREADS 0
#endif

/**
 * @def FQ_QUEUE_MIN_CAPACITY
 * @brief Minimum capacity for internal task queues.
 */
#if !defined(FQ_QUEUE_MIN_CAPACITY)
#    define FQ_QUEUE_MIN_CAPACITY 64
#endif

/**
 * @def FQ_QUEUE_DEFAULT_CAPACITY
 * @brief Default capacity for internal task queues.
 */
#if !defined(FQ_QUEUE_DEFAULT_CAPACITY)
#    define FQ_QUEUE_DEFAULT_CAPACITY 1024
#endif

/**
 * @def FQ_QUEUE_MAX_CAPACITY
 * @brief Maximum allowed capacity for task queues.
 */
#if !defined(FQ_QUEUE_MAX_CAPACITY)
#    define FQ_QUEUE_MAX_CAPACITY (1u << 24)
#endif

/**
 * @def FQ_FUTURE_SPIN_COUNT
 * @brief Number of spin iterations before a future falls back to the
 *        condition variable.
 */
#if !defined(FQ_FUTURE_SPIN_COUNT)
#    define FQ_FUTURE_SPIN_COUNT 128
#endif

/**
 * @def FQ_SHUTDOWN_TIMEOUT_MS
 * @brief Default timeout for graceful shutdown (milliseconds).
 */
#if !defined(FQ_SHUTDOWN_TIMEOUT_MS)
#    define FQ_SHUTDOWN_TIMEOUT_MS 5000
#endif

/**
 * @def FQ_CACHE_LINE_SIZE
 * @brief Cache-line size in bytes for padding.
 */
#if !defined(FQ_CACHE_LINE_SIZE)
#    define FQ_CACHE_LINE_SIZE 64
#endif

/**
 * @def FQ_LOG_LEVEL_NONE
 * @brief Disable all logging.
 */
#define FQ_LOG_LEVEL_NONE  0

/**
 * @def FQ_LOG_LEVEL_ERROR
 * @brief Log only errors.
 */
#define FQ_LOG_LEVEL_ERROR 1

/**
 * @def FQ_LOG_LEVEL_WARN
 * @brief Log warnings and errors.
 */
#define FQ_LOG_LEVEL_WARN  2

/**
 * @def FQ_LOG_LEVEL_INFO
 * @brief Log informational messages, warnings, and errors.
 */
#define FQ_LOG_LEVEL_INFO  3

/**
 * @def FQ_LOG_LEVEL_DEBUG
 * @brief Log everything (debug, info, warn, error).
 */
#define FQ_LOG_LEVEL_DEBUG 4

/**
 * @def FQ_DEFAULT_LOG_LEVEL
 * @brief Default log level at compile time.
 */
#if !defined(FQ_DEFAULT_LOG_LEVEL)
#    if defined(NDEBUG)
#        define FQ_DEFAULT_LOG_LEVEL FQ_LOG_LEVEL_WARN
#    else
#        define FQ_DEFAULT_LOG_LEVEL FQ_LOG_LEVEL_INFO
#    endif
#endif

/**
 * @def FQ_MAX_FUTURES
 * @brief Maximum number of futures that can be pooled.
 */
#if !defined(FQ_MAX_FUTURES)
#    define FQ_MAX_FUTURES 4096
#endif

/**
 * @def FQ_THREAD_STACK_SIZE
 * @brief Default thread stack size in bytes (0 = OS default).
 */
#if !defined(FQ_THREAD_STACK_SIZE)
#    define FQ_THREAD_STACK_SIZE 0
#endif

#endif /* FQ_CONFIG_H */
