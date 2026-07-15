/**
 * @file thread_pool.h
 * @brief High-level thread pool API built on top of the scheduler.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_THREAD_POOL_H
#define FQ_THREAD_POOL_H

#include "types.h"
#include "scheduler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a thread pool with default settings.
 *
 * Equivalent to creating a scheduler with default configuration.
 *
 * @param[out] pool Pointer to receive the new pool.
 * @return ::FQ_OK on success, or a negative error code.
 */
fq_status_t fq_thread_pool_create(fq_thread_pool_t **pool);

/**
 * @brief Create a thread pool with explicit thread count.
 *
 * @param[out] pool        Pointer to receive the new pool.
 * @param      thread_count Number of worker threads.
 * @return ::FQ_OK on success, or a negative error code.
 */
fq_status_t fq_thread_pool_create_ex(fq_thread_pool_t **pool,
                                     unsigned thread_count);

/**
 * @brief Create a thread pool from a full configuration.
 *
 * @param[out] pool   Pointer to receive the new pool.
 * @param      config Scheduler configuration.
 * @return ::FQ_OK on success, or a negative error code.
 */
fq_status_t fq_thread_pool_create_configured(
    fq_thread_pool_t **pool,
    const fq_scheduler_config_t *config);

/**
 * @brief Gracefully shut down a thread pool.
 *
 * Waits for all pending tasks to finish.
 *
 * @param pool Thread pool.
 * @return ::FQ_OK on success.
 */
fq_status_t fq_thread_pool_shutdown(fq_thread_pool_t *pool);

/**
 * @brief Submit a task to the thread pool.
 *
 * @param pool Thread pool.
 * @param task Task to submit.
 * @return ::FQ_OK on success, ::FQ_ERR_CLOSED.
 */
fq_status_t fq_thread_pool_submit(fq_thread_pool_t *pool, fq_task_t *task);

/**
 * @brief Submit a function as a task.
 *
 * @param pool      Thread pool.
 * @param fn        Function to execute.
 * @param user_data Argument forwarded to @p fn.
 * @return ::FQ_OK on success.
 */
fq_status_t fq_thread_pool_submit_fn(fq_thread_pool_t *pool,
                                     fq_task_fn fn, void *user_data);

/**
 * @brief Submit a task and get a future.
 *
 * @param pool      Thread pool.
 * @param task      Task to submit.
 * @param[out] future Pointer to receive the future (may be NULL).
 * @return ::FQ_OK on success.
 */
fq_status_t fq_thread_pool_submit_with_future(fq_thread_pool_t *pool,
                                              fq_task_t *task,
                                              fq_future_t **future);

/**
 * @brief Wait for all tasks to complete.
 *
 * @param pool Thread pool.
 */
void fq_thread_pool_wait_idle(fq_thread_pool_t *pool);

/**
 * @brief Check whether the pool is idle.
 *
 * @param pool Thread pool.
 * @return ::FQ_TRUE if no tasks are pending or executing.
 */
fq_bool_t fq_thread_pool_is_idle(const fq_thread_pool_t *pool);

/**
 * @brief Get the number of worker threads.
 *
 * @param pool Thread pool.
 * @return Thread count.
 */
unsigned fq_thread_pool_size(const fq_thread_pool_t *pool);

/**
 * @brief Retrieve pool statistics.
 *
 * @param pool  Thread pool.
 * @param[out] stats Pointer to fill.
 */
void fq_thread_pool_stats(const fq_thread_pool_t *pool,
                          fq_scheduler_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif /* FQ_THREAD_POOL_H */
