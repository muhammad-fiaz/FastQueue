/**
 * @file task.h
 * @brief Task creation and management for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_TASK_H
#define FQ_TASK_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new task with a function and user data.
 *
 * @param[out] task      Pointer to receive the new task.
 * @param      callback  Function to invoke when the task is executed.
 * @param      user_data Opaque pointer forwarded to @p callback.
 * @param      allocator Allocator (NULL for default).
 * @return ::FQ_OK on success, ::FQ_ERR_NOMEM.
 */
fq_status_t fq_task_create(fq_task_t **task, fq_task_fn callback,
                           void *user_data, const fq_allocator_t *allocator);

/**
 * @brief Create a task with a completion callback.
 *
 * @param[out] task       Pointer to receive the new task.
 * @param      callback   Function to invoke when the task is executed.
 * @param      user_data  Opaque pointer forwarded to @p callback.
 * @param      completion Completion callback (may be NULL).
 * @param      completion_data Data forwarded to @p completion.
 * @param      allocator  Allocator (NULL for default).
 * @return ::FQ_OK on success, ::FQ_ERR_NOMEM.
 */
fq_status_t fq_task_create_with_completion(fq_task_t **task,
                                           fq_task_fn callback,
                                           void *user_data,
                                           fq_completion_fn completion,
                                           void *completion_data,
                                           const fq_allocator_t *allocator);

/**
 * @brief Destroy a task and release its memory.
 *
 * If the task has a future, the future's completion callback is invoked
 * with ::FQ_ERR_CANCELED.
 *
 * @param task Task to destroy (NULL is a safe no-op).
 */
void fq_task_destroy(fq_task_t *task);

/**
 * @brief Retrieve the user data pointer of a task.
 *
 * @param task Task.
 * @return The user data pointer.
 */
void *fq_task_user_data(const fq_task_t *task);

/**
 * @brief Retrieve the priority of a task.
 *
 * @param task Task.
 * @return Priority level.
 */
fq_priority_t fq_task_priority(const fq_task_t *task);

/**
 * @brief Set the priority of a task.
 *
 * @param task     Task.
 * @param priority New priority level.
 */
void fq_task_set_priority(fq_task_t *task, fq_priority_t priority);

/**
 * @brief Attach a future to a task.
 *
 * @param task    Task.
 * @param future  Future to attach (NULL to detach).
 */
void fq_task_set_future(fq_task_t *task, fq_future_t *future);

/**
 * @brief Retrieve the future attached to a task.
 *
 * @param task Task.
 * @return Attached future, or NULL.
 */
fq_future_t *fq_task_future(const fq_task_t *task);

/**
 * @brief Retrieve the execution state of a task.
 *
 * @param task Task.
 * @return ::FQ_TRUE if the task has been (or is being) executed.
 */
fq_bool_t fq_task_is_executed(const fq_task_t *task);

/**
 * @brief Retrieve the cancel state of a task.
 *
 * @param task Task.
 * @return ::FQ_TRUE if the task has been canceled.
 */
fq_bool_t fq_task_is_canceled(const fq_task_t *task);

#ifdef __cplusplus
}
#endif

#endif /* FQ_TASK_H */
