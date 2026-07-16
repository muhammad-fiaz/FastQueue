/**
 * @file types.h
 * @brief Core type definitions for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_TYPES_H
#define FQ_TYPES_H

#include "config.h"
#include "platform.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

 

/**
 * @enum fq_status_t
 * @brief Operation status codes.
 */
typedef enum fq_status_t {
    FQ_OK            =  0,  /**< Success. */
    FQ_ERR_NOMEM     = -1,  /**< Out of memory. */
    FQ_ERR_INVAL     = -2,  /**< Invalid argument. */
    FQ_ERR_BUSY      = -3,  /**< Resource busy / would block. */
    FQ_ERR_TIMEOUT   = -4,  /**< Operation timed out. */
    FQ_ERR_CANCELED  = -5,  /**< Operation was canceled. */
    FQ_ERR_OVERFLOW  = -6,  /**< Queue is full. */
    FQ_ERR_CLOSED    = -7,  /**< System has been shut down. */
    FQ_ERR_INTERNAL  = -8,  /**< Internal error. */
    FQ_ERR_NOSUPPORT = -9   /**< Operation not supported. */
} fq_status_t;

 

/**
 * @struct fq_task_t
 * @brief A unit of work submitted to the system.
 */
typedef struct fq_task_t fq_task_t;

/**
 * @struct fq_future_t
 * @brief Handle to a pending result of an asynchronous operation.
 */
typedef struct fq_future_t fq_future_t;

/**
 * @struct fq_queue_t
 * @brief Thread-safe FIFO task queue.
 */
typedef struct fq_queue_t fq_queue_t;

/**
 * @struct fq_scheduler_t
 * @brief Job scheduler that distributes tasks across worker threads.
 */
typedef struct fq_scheduler_t fq_scheduler_t;

/**
 * @struct fq_thread_pool_t
 * @brief High-level thread pool built on top of the scheduler.
 */
typedef struct fq_scheduler_t fq_thread_pool_t;

 

/**
 * @enum fq_priority_t
 * @brief Task priority levels (higher numeric = higher priority).
 */
typedef enum fq_priority_t {
    FQ_PRIORITY_LOW    = 0,  /**< Low priority. */
    FQ_PRIORITY_NORMAL = 1,  /**< Normal (default). */
    FQ_PRIORITY_HIGH   = 2,  /**< High priority. */
    FQ_PRIORITY_URGENT = 3   /**< Urgent / realtime-like. */
} fq_priority_t;

 

/**
 * @brief Task callback function pointer.
 *
 * @param user_data Opaque pointer provided at submission time.
 */
typedef void (*fq_task_fn)(void *user_data);

/**
 * @brief Completion callback invoked when a future is fulfilled.
 *
 * @param user_data Opaque pointer provided at creation time.
 * @param status    Status of the completed operation.
 */
typedef void (*fq_completion_fn)(void *user_data, fq_status_t status);

 

/**
 * @struct fq_allocator_t
 * @brief Custom allocator interface.
 *
 * All function pointers are required (no NULL defaults).  The @c ctx
 * pointer is forwarded as the first argument to each function.
 */
typedef struct fq_allocator_t {
    void *(*alloc)(size_t size, void *ctx);     /**< Allocate @p size bytes. */
    void *(*realloc)(void *ptr, size_t size, void *ctx); /**< Resize. */
    void  (*free)(void *ptr, void *ctx);         /**< Free memory. */
    void  *ctx;                                  /**< User context. */
} fq_allocator_t;

 

/**
 * @enum fq_log_level_t
 * @brief Log severity levels.
 */
typedef enum fq_log_level_t {
    FQ_LOG_NONE  = FQ_LOG_LEVEL_NONE,
    FQ_LOG_ERROR = FQ_LOG_LEVEL_ERROR,
    FQ_LOG_WARN  = FQ_LOG_LEVEL_WARN,
    FQ_LOG_INFO  = FQ_LOG_LEVEL_INFO,
    FQ_LOG_DEBUG = FQ_LOG_LEVEL_DEBUG
} fq_log_level_t;

/**
 * @brief Logging callback function pointer.
 *
 * @param level   Severity of the message.
 * @param message Null-terminated log message.
 * @param ctx     User context pointer.
 */
typedef void (*fq_log_fn)(fq_log_level_t level, const char *message, void *ctx);

 

/**
 * @typedef fq_bool_t
 * @brief Boolean type (0 = false, 1 = true).
 */
typedef int fq_bool_t;

#define FQ_TRUE  1
#define FQ_FALSE 0

/**
 * @typedef fq_usize_t
 * @brief Unsigned size type matching the platform pointer width.
 */
typedef size_t fq_usize_t;

/**
 * @typedef fq_isize_t
 * @brief Signed size type.
 */
typedef ptrdiff_t fq_isize_t;

#ifdef __cplusplus
}
#endif

#endif /* FQ_TYPES_H */
