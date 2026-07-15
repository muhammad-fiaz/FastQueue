/**
 * @file memory.c
 * @brief Default allocator and memory statistics.
 */

#include "fastqueue/memory.h"
#include "fastqueue/atomic.h"

#include <stdlib.h>
#include <string.h>

/* ── Global statistics ───────────────────────────────────────────────── */

static fq_atomic_int_t g_alloc_count  = FQ_ATOMIC_INIT(0);
static fq_atomic_int_t g_free_count   = FQ_ATOMIC_INIT(0);
static fq_atomic_int_t g_allocated_32 = FQ_ATOMIC_INIT(0);
static fq_atomic_int_t g_peak_32      = FQ_ATOMIC_INIT(0);

static void stats_alloc(size_t size)
{
    int sz32 = (int)(size & 0x7FFFFFFF);
    int prev = fq_atomic_fetch_add_explicit(
        &g_allocated_32, sz32, FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_fetch_add_explicit(&g_alloc_count, 1, FQ_MEMORY_ORDER_RELAXED);
    int new_val = prev + sz32;
    /* Update peak via CAS loop. */
    for (;;) {
        int p = fq_atomic_load_explicit(&g_peak_32, FQ_MEMORY_ORDER_RELAXED);
        if (new_val <= p) break;
        if (fq_atomic_compare_exchange_weak_explicit(
                &g_peak_32, &p, new_val,
                FQ_MEMORY_ORDER_RELAXED, FQ_MEMORY_ORDER_RELAXED)) {
            break;
        }
    }
}

static void stats_free(size_t size)
{
    int sz32 = (int)(size & 0x7FFFFFFF);
    fq_atomic_fetch_sub_explicit(&g_allocated_32, sz32,
                                 FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_fetch_add_explicit(&g_free_count, 1, FQ_MEMORY_ORDER_RELAXED);
}

/* ── Default allocator ───────────────────────────────────────────────── */

static void *default_alloc(size_t size, void *ctx)
{
    (void)ctx;
    void *p = malloc(size);
    if (p) stats_alloc(size);
    return p;
}

static void *default_realloc(void *ptr, size_t size, void *ctx)
{
    (void)ctx;
    void *p = realloc(ptr, size);
    if (p && !ptr) stats_alloc(size);
    return p;
}

static void default_free(void *ptr, void *ctx)
{
    (void)ctx;
    if (ptr) {
        stats_free(0);
        free(ptr);
    }
}

static const fq_allocator_t g_default_allocator = {
    .alloc   = default_alloc,
    .realloc = default_realloc,
    .free    = default_free,
    .ctx     = NULL,
};

const fq_allocator_t *fq_default_allocator(void)
{
    return &g_default_allocator;
}

/* ── Public wrappers ─────────────────────────────────────────────────── */

void *fq_alloc(const fq_allocator_t *alloc, size_t size)
{
    if (!alloc) alloc = &g_default_allocator;
    return alloc->alloc(size, alloc->ctx);
}

void *fq_calloc(const fq_allocator_t *alloc, size_t count, size_t size)
{
    if (!alloc) alloc = &g_default_allocator;
    size_t total = count * size;
    void *p = alloc->alloc(total, alloc->ctx);
    if (p) memset(p, 0, total);
    return p;
}

void *fq_realloc(const fq_allocator_t *alloc, void *ptr, size_t size)
{
    if (!alloc) alloc = &g_default_allocator;
    return alloc->realloc(ptr, size, alloc->ctx);
}

void fq_free(const fq_allocator_t *alloc, void *ptr)
{
    if (!alloc) alloc = &g_default_allocator;
    alloc->free(ptr, alloc->ctx);
}

/* ── Alignment ───────────────────────────────────────────────────────── */

size_t fq_align_up(size_t size, size_t alignment)
{
    if (alignment == 0 || (alignment & (alignment - 1)) != 0)
        return size;
    return (size + alignment - 1) & ~(alignment - 1);
}

fq_bool_t fq_is_aligned(const void *ptr, size_t alignment)
{
    if (alignment == 0) return FQ_TRUE;
    return ((uintptr_t)ptr & (alignment - 1)) == 0;
}

/* ── Statistics ──────────────────────────────────────────────────────── */

void fq_mem_stats(fq_mem_stats_t *stats)
{
    if (!stats) return;
    stats->allocated_bytes = (size_t)(unsigned)fq_atomic_load_explicit(
        &g_allocated_32, FQ_MEMORY_ORDER_RELAXED);
    stats->total_allocs = (size_t)(unsigned)fq_atomic_load_explicit(
        &g_alloc_count, FQ_MEMORY_ORDER_RELAXED);
    stats->total_frees = (size_t)(unsigned)fq_atomic_load_explicit(
        &g_free_count, FQ_MEMORY_ORDER_RELAXED);
    stats->peak_bytes = (size_t)(unsigned)fq_atomic_load_explicit(
        &g_peak_32, FQ_MEMORY_ORDER_RELAXED);
}
