---
url: /FastQueue/api/parallel.md
description: >-
  FastQueue parallel for-loop utility API reference for distributing iterations
  across worker threads.
---

# Parallel For API

Distributes a loop across worker threads for automatic data-parallel execution.

## Types

```c
typedef void (*fq_parallel_for_fn)(long index, void *ctx);
```

Callback invoked for each iteration index.

## Functions

### Scheduler-based

```c
fq_status_t fq_scheduler_parallel_for(
    fq_scheduler_t *scheduler,
    long begin,
    long end,
    fq_parallel_for_fn fn,
    void *ctx
);
```

Execute a function over the range `[begin, end)` in parallel using the scheduler's worker threads. Blocks until all iterations complete.

The range is split into chunks proportional to the number of workers.

### Thread Pool-based

```c
fq_status_t fq_thread_pool_parallel_for(
    fq_thread_pool_t *pool,
    long begin,
    long end,
    fq_parallel_for_fn fn,
    void *ctx
);
```

Same as above but accepts a thread pool directly.

## Example

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double *data;
    double  scalar;
} scale_ctx_t;

static void scale_element(long i, void *arg)
{
    scale_ctx_t *ctx = arg;
    ctx->data[i] *= ctx->scalar;
}

int main(void)
{
    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create(&pool);

    const long N = 1000000;
    double *arr = malloc(N * sizeof(double));
    for (long i = 0; i < N; i++) arr[i] = (double)i;

    scale_ctx_t ctx = { .data = arr, .scalar = 2.0 };
    fq_thread_pool_parallel_for(pool, 0, N, scale_element, &ctx);

    fq_thread_pool_shutdown(pool);
    free(arr);
    return 0;
}
```
