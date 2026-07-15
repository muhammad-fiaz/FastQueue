/**
 * @file scheduler.h
 * @brief Job scheduler that distributes tasks across worker threads.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_SCHEDULER_H
#define FQ_SCHEDULER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configuration for the scheduler.
 */
typedef struct fq_scheduler_config_t {
    unsigned        thread_count;     /**< Worker threads (0 = auto). */
    size_t          queue_capacity;   /**< Per-thread queue capacity. */
    fq_bool_t       enable_work_stealing; /**< Enable work-stealing. */
    fq_bool_t       enable_priority;  /**< Enable priority scheduling. */
    const fq_allocator_t *allocator;  /**< Custom allocator (NULL = default). */
    fq_log_fn       log_callback;     /**< Logging callback (NULL = none). */
    void           *log_context;      /**< Context for @p log_callback. */
    fq_log_level_t  log_level;        /**< Minimum log level. */
} fq_scheduler_config_t;

/**
 * @brief Initialise a scheduler with default configuration.
 *
 * @param[out] config Pointer to configuration struct to fill.
 */
void fq_scheduler_config_default(fq_scheduler_config_t *config);

/**
 * @brief Create and start a scheduler.
 *
 * @param[out] scheduler Pointer to receive the new scheduler.
 * @param      config    Configuration (NULL for defaults).
 * @return ::FQ_OK on success, or a negative error code.
 */
fq_status_t fq_scheduler_create(fq_scheduler_t **scheduler,
                                const fq_scheduler_config_t *config);

/**
 * @brief Gracefully shut down a scheduler.
 *
 * Waits for all pending tasks to complete, then stops worker threads.
 *
 * @param scheduler Scheduler.
 * @return ::FQ_OK on success.
 */
fq_status_t fq_scheduler_shutdown(fq_scheduler_t *scheduler);

/**
 * @brief Forcibly cancel all pending tasks and shut down.
 *
 * @param scheduler Scheduler.
 */
void fq_scheduler_cancel_all(fq_scheduler_t *scheduler);

/**
 * @brief Submit a task to the scheduler.
 *
 * The scheduler takes ownership of the task pointer.  Do not call
 * fq_task_destroy() on a submitted task.
 *
 * @param scheduler Scheduler.
 * @param task      Task to submit (non-NULL, not yet submitted).
 * @return ::FQ_OK on success, ::FQ_ERR_CLOSED, ::FQ_ERR_OVERFLOW.
 */
fq_status_t fq_scheduler_submit(fq_scheduler_t *scheduler, fq_task_t *task);

/**
 * @brief Submit a task and return a future for its result.
 *
 * @param scheduler Scheduler.
 * @param task      Task to submit.
 * @param[out] future Pointer to receive the future (may be NULL).
 * @return ::FQ_OK on success.
 */
fq_status_t fq_scheduler_submit_with_future(fq_scheduler_t *scheduler,
                                            fq_task_t *task,
                                            fq_future_t **future);

/**
 * @brief Submit a simple function as a task.
 *
 * Convenience wrapper around fq_scheduler_submit().
 *
 * @param scheduler Scheduler.
 * @param fn        Function to execute.
 * @param user_data Argument forwarded to @p fn.
 * @return ::FQ_OK on success.
 */
fq_status_t fq_scheduler_submit_fn(fq_scheduler_t *scheduler,
                                   fq_task_fn fn, void *user_data);

/**
 * @brief Wait for all pending tasks to drain.
 *
 * @param scheduler Scheduler.
 */
void fq_scheduler_wait_idle(fq_scheduler_t *scheduler);

/**
 * @brief Check whether the scheduler is idle.
 *
 * @param scheduler Scheduler.
 * @return ::FQ_TRUE if no tasks are pending or executing.
 */
fq_bool_t fq_scheduler_is_idle(const fq_scheduler_t *scheduler);

/**
 * @brief Check whether the scheduler has been shut down.
 *
 * @param scheduler Scheduler.
 * @return ::FQ_TRUE if shut down.
 */
fq_bool_t fq_scheduler_is_shutdown(const fq_scheduler_t *scheduler);

/* ── Statistics ──────────────────────────────────────────────────────── */

/**
 * @struct fq_scheduler_stats_t
 * @brief Runtime statistics of a scheduler.
 */
typedef struct fq_scheduler_stats_t {
    unsigned thread_count;       /**< Number of worker threads. */
    size_t   tasks_submitted;    /**< Total tasks submitted. */
    size_t   tasks_completed;    /**< Total tasks completed. */
    size_t   tasks_canceled;     /**< Total tasks canceled. */
    size_t   tasks_pending;      /**< Currently pending tasks. */
    size_t   tasks_active;       /**< Currently executing tasks. */
    uint64_t total_wait_ns;      /**< Cumulative wait time in nanoseconds. */
    uint64_t total_work_ns;      /**< Cumulative work time in nanoseconds. */
} fq_scheduler_stats_t;

/**
 * @brief Retrieve scheduler statistics.
 *
 * @param scheduler Scheduler.
 * @param[out] stats Pointer to fill.
 */
void fq_scheduler_stats(const fq_scheduler_t *scheduler,
                        fq_scheduler_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif /* FQ_SCHEDULER_H */
