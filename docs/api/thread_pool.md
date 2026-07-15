---
title: Thread Pool API
description: FastQueue thread pool API reference with creation, submission, and lifecycle functions.
keywords: thread pool api, fq_thread_pool, create, submit, shutdown
---

# Thread Pool API

High-level thread pool interface wrapping the scheduler.

## Types

```c
typedef struct fq_thread_pool_t fq_thread_pool_t;
```

## Functions

### Creation

```c
fq_status_t fq_thread_pool_create(fq_thread_pool_t **pool);
```
Create a thread pool with default settings (auto-detect thread count).

```c
fq_status_t fq_thread_pool_create_ex(fq_thread_pool_t **pool, unsigned thread_count);
```
Create a thread pool with specific thread count.

```c
fq_status_t fq_thread_pool_create_configured(
    fq_thread_pool_t **pool,
    const fq_scheduler_config_t *config
);
```
Create a thread pool with full configuration.

### Task Submission

```c
fq_status_t fq_thread_pool_submit_fn(
    fq_thread_pool_t *pool,
    fq_task_fn fn,
    void *user_data
);
```
Submit a function pointer as a task.

```c
fq_status_t fq_thread_pool_submit(fq_thread_pool_t *pool, fq_task_t *task);
```
Submit a pre-built task.

```c
fq_status_t fq_thread_pool_submit_with_future(
    fq_thread_pool_t *pool,
    fq_task_t *task,
    fq_future_t **future
);
```
Submit a task and get a future for the result.

### Lifecycle

```c
void fq_thread_pool_wait_idle(fq_thread_pool_t *pool);
```
Block until all submitted tasks complete.

```c
fq_bool_t fq_thread_pool_is_idle(const fq_thread_pool_t *pool);
```
Check if the pool is idle.

```c
unsigned fq_thread_pool_size(const fq_thread_pool_t *pool);
```
Get the number of worker threads.

```c
fq_status_t fq_thread_pool_shutdown(fq_thread_pool_t *pool);
```
Shutdown the pool and free resources.

### Statistics

```c
void fq_thread_pool_stats(const fq_thread_pool_t *pool, fq_thread_pool_stats_t *stats);
```
Get pool statistics.

## Example

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void task(void *arg)
{
    printf("Task %d\n", *(int *)arg);
}

int main(void)
{
    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, 4);

    for (int i = 0; i < 100; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        fq_thread_pool_submit_fn(pool, task, id);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```
