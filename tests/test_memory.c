/**
 * @file test_memory.c
 * @brief Memory allocator tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>
#include <string.h>

int test_memory_alloc(void)
{
    const fq_allocator_t *def = fq_default_allocator();
    if (!def) return 1;

    /* Basic alloc / free. */
    void *p = fq_alloc(def, 128);
    if (!p) return 2;
    memset(p, 0xAB, 128);
    fq_free(def, p);

    /* Calloc. */
    int *arr = (int *)fq_calloc(def, 10, sizeof(int));
    if (!arr) return 3;
    for (int i = 0; i < 10; ++i) {
        if (arr[i] != 0) { fq_free(def, arr); return 4; }
    }
    fq_free(def, arr);

    /* Realloc. */
    p = fq_alloc(def, 64);
    if (!p) return 5;
    p = fq_realloc(def, p, 128);
    if (!p) return 6;
    fq_free(def, p);

    /* Alignment. */
    if (fq_align_up(1, 8) != 8) return 7;
    if (fq_align_up(7, 8) != 8) return 8;
    if (fq_align_up(8, 8) != 8) return 9;
    if (fq_align_up(9, 8) != 16) return 10;

    /* Is-aligned. */
    int x;
    if (!fq_is_aligned(&x, 1)) return 11;

    /* NULL free is safe. */
    fq_free(def, NULL);

    /* Statistics. */
    fq_mem_stats_t stats;
    fq_mem_stats(&stats);
    /* Just verify it doesn't crash. */
    (void)stats;

    return 0;
}
