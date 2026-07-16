/**
 * @file parallel.c
 * @brief Parallel for-loop implementation.
 */

#include "fastqueue/parallel.h"
#include "fastqueue/task.h"
#include "fastqueue/memory.h"

#include <string.h>

typedef struct {
    fq_parallel_for_fn fn;
    void              *ctx;
    long               begin;
    long               end;
} fq_parallel_range_t;

static void parallel_for_worker(void *arg)
{
    fq_parallel_range_t *range = (fq_parallel_range_t *)arg;
    fq_parallel_for_fn fn = range->fn;
    void *ctx = range->ctx;
    long begin = range->begin;
    long end = range->end;
    fq_free(fq_default_allocator(), range);
    for (long i = begin; i < end; ++i) {
        fn(i, ctx);
    }
}

fq_status_t fq_scheduler_parallel_for(fq_scheduler_t *scheduler,
                                       long begin, long end,
                                       fq_parallel_for_fn fn,
                                       void *ctx)
{
    if (!scheduler || !fn || begin >= end) return FQ_ERR_INVAL;

    long count = end - begin;
    fq_scheduler_stats_t stats;
    fq_scheduler_stats(scheduler, &stats);
    unsigned threads = stats.thread_count;
    if (threads == 0) threads = 1;

    long chunk = count / (long)threads;
    long remainder = count % (long)threads;

    fq_status_t st = FQ_OK;
    long cur = begin;

    for (unsigned i = 0; i < threads && cur < end; ++i) {
        long chunk_end = cur + chunk + (long)(i < (unsigned)remainder ? 1 : 0);
        if (chunk_end > end) chunk_end = end;

        fq_parallel_range_t *range = (fq_parallel_range_t *)fq_alloc(
            fq_default_allocator(), sizeof(fq_parallel_range_t));
        if (!range) {
            st = FQ_ERR_NOMEM;
            break;
        }

        range->fn    = fn;
        range->ctx   = ctx;
        range->begin = cur;
        range->end   = chunk_end;

        st = fq_scheduler_submit_fn(scheduler, parallel_for_worker, range);
        if (st != FQ_OK) {
            fq_free(fq_default_allocator(), range);
            break;
        }
        cur = chunk_end;
    }

    if (st == FQ_OK) {
        fq_scheduler_wait_idle(scheduler);
    }

    return st;
}

fq_status_t fq_thread_pool_parallel_for(fq_thread_pool_t *pool,
                                         long begin, long end,
                                         fq_parallel_for_fn fn,
                                         void *ctx)
{
    return fq_scheduler_parallel_for((fq_scheduler_t *)pool,
                                     begin, end, fn, ctx);
}
