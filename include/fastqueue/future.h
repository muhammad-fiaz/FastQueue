/**
 * @file future.h
 * @brief Futures and promise mechanism for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_FUTURE_H
#define FQ_FUTURE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new future.
 *
 * @param[out] future Pointer to receive the new future.
 * @param      allocator Allocator (NULL for default).
 * @return ::FQ_OK on success, ::FQ_ERR_NOMEM.
 */
fq_status_t fq_future_create(fq_future_t **future,
                             const fq_allocator_t *allocator);

/**
 * @brief Destroy a future.
 *
 * If the future is still pending, its completion callback is invoked
 * with ::FQ_ERR_CANCELED.
 *
 * @param future Future to destroy (NULL is a safe no-op).
 */
void fq_future_destroy(fq_future_t *future);

/**
 * @brief Block the calling thread until the future is fulfilled.
 *
 * Uses a spin-then-wait strategy to minimise latency while avoiding
 * busy-waiting.
 *
 * @param future Future to wait on.
 * @return Status of the completed operation.
 */
fq_status_t fq_future_wait(fq_future_t *future);

/**
 * @brief Wait with a timeout.
 *
 * @param future    Future to wait on.
 * @param timeout_ms Maximum time to wait in milliseconds.
 * @param[out] status Pointer to receive the result status (may be NULL).
 * @return ::FQ_OK on success, ::FQ_ERR_TIMEOUT.
 */
fq_status_t fq_future_wait_timeout(fq_future_t *future,
                                   unsigned timeout_ms,
                                   fq_status_t *status);

/**
 * @brief Check whether a future is fulfilled.
 *
 * @param future Future.
 * @return ::FQ_TRUE if fulfilled (success or error), ::FQ_FALSE otherwise.
 */
fq_bool_t fq_future_is_ready(const fq_future_t *future);

/**
 * @brief Check whether a future was canceled.
 *
 * @param future Future.
 * @return ::FQ_TRUE if canceled.
 */
fq_bool_t fq_future_is_canceled(const fq_future_t *future);

/**
 * @brief Retrieve the status of a fulfilled future.
 *
 * @param future Future (must be ready).
 * @return Status code, or ::FQ_ERR_BUSY if still pending.
 */
fq_status_t fq_future_status(const fq_future_t *future);

/**
 * @brief Attach a completion callback to a future.
 *
 * If the future is already fulfilled the callback is invoked immediately
 * on the calling thread.
 *
 * @param future     Future.
 * @param callback   Completion function.
 * @param user_data  Opaque pointer forwarded to @p callback.
 * @return ::FQ_OK on success.
 */
fq_status_t fq_future_on_complete(fq_future_t *future,
                                  fq_completion_fn callback,
                                  void *user_data);

/**
 * @brief Cancel a pending future.
 *
 * The future's completion callback (if any) is invoked with
 * ::FQ_ERR_CANCELED.
 *
 * @param future Future.
 * @return ::FQ_OK on success, ::FQ_ERR_CLOSED if already fulfilled.
 */
fq_status_t fq_future_cancel(fq_future_t *future);

 

/**
 * @brief Fulfill a future with a result status.
 *
 * This is called internally by the scheduler.  Users should normally
 * not need to call this directly.
 *
 * @param future Future to fulfill.
 * @param status Result status.
 */
void fq_future_set_result(fq_future_t *future, fq_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* FQ_FUTURE_H */
