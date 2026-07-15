/**
 * @file memory.h
 * @brief Custom allocator interface and memory utilities for FastQueue.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_MEMORY_H
#define FQ_MEMORY_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

 

/**
 * @brief Return the default system allocator.
 *
 * Uses standard malloc / calloc / realloc / free.
 *
 * @return Pointer to a static allocator struct.
 */
const fq_allocator_t *fq_default_allocator(void);

/**
 * @brief Allocate memory using the given allocator.
 *
 * @param alloc     Allocator (NULL for default).
 * @param size      Number of bytes to allocate.
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *fq_alloc(const fq_allocator_t *alloc, size_t size);

/**
 * @brief Allocate zeroed memory.
 *
 * @param alloc Allocator (NULL for default).
 * @param count Element count.
 * @param size  Size of each element.
 * @return Pointer to zeroed memory, or NULL on failure.
 */
void *fq_calloc(const fq_allocator_t *alloc, size_t count, size_t size);

/**
 * @brief Reallocate memory.
 *
 * @param alloc Allocator (NULL for default).
 * @param ptr   Previous allocation (may be NULL).
 * @param size  New size in bytes.
 * @return Pointer to resized memory, or NULL on failure.
 */
void *fq_realloc(const fq_allocator_t *alloc, void *ptr, size_t size);

/**
 * @brief Free memory.
 *
 * @param alloc Allocator (NULL for default).
 * @param ptr   Pointer to free (NULL is a safe no-op).
 */
void fq_free(const fq_allocator_t *alloc, void *ptr);

 

/**
 * @brief Round @p size up to the nearest multiple of @p alignment.
 *
 * @param size       Unaligned size.
 * @param alignment  Required alignment (must be a power of 2).
 * @return Aligned size >= @p size.
 */
size_t fq_align_up(size_t size, size_t alignment);

/**
 * @brief Check whether @p ptr is aligned to @p alignment.
 *
 * @param ptr        Pointer to test.
 * @param alignment  Required alignment (must be a power of 2).
 * @return ::FQ_TRUE if aligned, ::FQ_FALSE otherwise.
 */
fq_bool_t fq_is_aligned(const void *ptr, size_t alignment);

 

/**
 * @struct fq_mem_stats_t
 * @brief Snapshot of memory allocation statistics.
 */
typedef struct fq_mem_stats_t {
    size_t allocated_bytes;  /**< Current bytes outstanding. */
    size_t total_allocs;    /**< Lifetime allocation count. */
    size_t total_frees;     /**< Lifetime free count. */
    size_t peak_bytes;      /**< High-water mark. */
} fq_mem_stats_t;

/**
 * @brief Retrieve global memory statistics.
 *
 * @param[out] stats Pointer to fill with the current snapshot.
 */
void fq_mem_stats(fq_mem_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif /* FQ_MEMORY_H */
