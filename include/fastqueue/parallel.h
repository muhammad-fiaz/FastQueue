/**
 * @file parallel.h
 * @brief Parallel for-loop utility for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_PARALLEL_H
#define FQ_PARALLEL_H

#include "types.h"
#include "scheduler.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback for parallel_for iteration.
 *
 * @param index   Current iteration index.
 * @param ctx     User context pointer.
 */
typedef void (*fq_parallel_for_fn)(long index, void *ctx);

/**
 * @brief Execute a function over a range of indices in parallel.
 *
 * Splits the range [begin, end) into chunks and distributes them across
 * the scheduler's worker threads.  Blocks until all iterations complete.
 *
 * @param scheduler Scheduler to use for parallelism.
 * @param begin     Start of the range (inclusive).
 * @param end       End of the range (exclusive).
 * @param fn        Function to call for each index.
 * @param ctx       User context passed to @p fn.
 *
 * @return ::FQ_OK on success, or an error code.
 *
 * @note The callback @p fn must be thread-safe.
 * @note If @p scheduler has N workers, the range is split into N chunks.
 */
FQ_API fq_status_t fq_scheduler_parallel_for(fq_scheduler_t *scheduler,
                                              long begin, long end,
                                              fq_parallel_for_fn fn,
                                              void *ctx);

/**
 * @brief Thread pool version of parallel_for.
 *
 * @param pool  Thread pool to use.
 * @param begin Start of the range (inclusive).
 * @param end   End of the range (exclusive).
 * @param fn    Function to call for each index.
 * @param ctx   User context passed to @p fn.
 *
 * @return ::FQ_OK on success, or an error code.
 */
FQ_API fq_status_t fq_thread_pool_parallel_for(fq_thread_pool_t *pool,
                                                long begin, long end,
                                                fq_parallel_for_fn fn,
                                                void *ctx);

#ifdef __cplusplus
}
#endif

#endif /* FQ_PARALLEL_H */
