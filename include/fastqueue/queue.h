/**
 * @file queue.h
 * @brief Lock-free SPSC and thread-safe MPMC task queues.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_QUEUE_H
#define FQ_QUEUE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new thread-safe task queue.
 *
 * @param[out] queue     Pointer to receive the new queue.
 * @param      capacity  Initial capacity (rounded up to power of 2).
 * @param      allocator Allocator to use (NULL for default).
 * @return ::FQ_OK on success, or a negative error code.
 */
fq_status_t fq_queue_create(fq_queue_t **queue, size_t capacity,
                            const fq_allocator_t *allocator);

/**
 * @brief Destroy a queue and release all associated memory.
 *
 * Pending tasks are **not** freed; the caller is responsible for draining
 * or discarding them before calling this function.
 *
 * @param queue Queue to destroy.
 */
void fq_queue_destroy(fq_queue_t *queue);

/**
 * @brief Push a task into the back of the queue.
 *
 * Thread-safe: may be called concurrently from multiple threads.
 *
 * @param queue Queue.
 * @param task  Task to enqueue (non-NULL).
 * @return ::FQ_OK on success, ::FQ_ERR_OVERFLOW if full, ::FQ_ERR_CLOSED.
 */
fq_status_t fq_queue_push(fq_queue_t *queue, fq_task_t *task);

/**
 * @brief Pop a task from the front of the queue.
 *
 * Thread-safe: may be called concurrently from multiple threads.
 *
 * @param queue Queue.
 * @param[out] task  Pointer to receive the popped task.
 * @return ::FQ_OK on success, ::FQ_ERR_BUSY if empty, ::FQ_ERR_CLOSED.
 */
fq_status_t fq_queue_pop(fq_queue_t *queue, fq_task_t **task);

/**
 * @brief Try to pop a task without blocking.
 *
 * @param queue Queue.
 * @param[out] task  Pointer to receive the popped task.
 * @return ::FQ_OK on success, ::FQ_ERR_BUSY if empty.
 */
fq_status_t fq_queue_try_pop(fq_queue_t *queue, fq_task_t **task);

/**
 * @brief Return the current number of tasks in the queue.
 *
 * @param queue Queue.
 * @return Approximate count (may be stale under contention).
 */
size_t fq_queue_size(const fq_queue_t *queue);

/**
 * @brief Check whether the queue is empty.
 *
 * @param queue Queue.
 * @return ::FQ_TRUE if empty, ::FQ_FALSE otherwise.
 */
fq_bool_t fq_queue_empty(const fq_queue_t *queue);

/**
 * @brief Return the capacity of the queue.
 *
 * @param queue Queue.
 * @return Maximum number of tasks that can be stored.
 */
size_t fq_queue_capacity(const fq_queue_t *queue);

/**
 * @brief Drain up to @p max tasks from the queue into @p out.
 *
 * Thread-safe: atomically moves up to @p max tasks.
 *
 * @param queue    Queue.
 * @param[out] out Array to receive task pointers.
 * @param max      Maximum number of tasks to drain.
 * @return Number of tasks actually drained.
 */
size_t fq_queue_drain(fq_queue_t *queue, fq_task_t **out, size_t max);

#ifdef __cplusplus
}
#endif

#endif /* FQ_QUEUE_H */
