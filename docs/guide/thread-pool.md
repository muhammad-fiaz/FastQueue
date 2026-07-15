---
title: Thread Pool
description: Learn how to configure and use the FastQueue thread pool with work stealing, task submission, and lifecycle management.
keywords: thread pool, work stealing, task submission, fq_thread_pool, c23
---

# Thread Pool

The thread pool is the primary interface for submitting and executing tasks concurrently.

## Basic Usage

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void worker(void *arg)
{
    int id = *(int *)arg;
    printf("Worker task %d\n", id);
}

int main(void)
{
    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, 4);

    for (int i = 0; i < 100; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        fq_thread_pool_submit_fn(pool, worker, id);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Configuration

```c
fq_scheduler_config_t cfg;
fq_scheduler_config_default(&cfg);

cfg.thread_count       = 8;     // Number of worker threads
cfg.queue_capacity     = 4096;  // Task queue capacity
cfg.enable_work_stealing = FQ_TRUE;  // Enable work stealing

fq_thread_pool_t *pool = NULL;
fq_thread_pool_create_configured(&pool, &cfg);
```

## API Reference

### Creation

```c
// Create with default settings
fq_status_t fq_thread_pool_create(fq_thread_pool_t **pool);

// Create with specific thread count
fq_status_t fq_thread_pool_create_ex(fq_thread_pool_t **pool, unsigned thread_count);

// Create with full configuration
fq_status_t fq_thread_pool_create_configured(
    fq_thread_pool_t **pool,
    const fq_scheduler_config_t *config
);
```

### Task Submission

```c
// Submit a function pointer
fq_status_t fq_thread_pool_submit_fn(
    fq_thread_pool_t *pool,
    fq_task_fn fn,
    void *user_data
);

// Submit a pre-built task
fq_status_t fq_thread_pool_submit(fq_thread_pool_t *pool, fq_task_t *task);

// Submit with future
fq_status_t fq_thread_pool_submit_with_future(
    fq_thread_pool_t *pool,
    fq_task_t *task,
    fq_future_t **future
);
```

### Lifecycle

```c
// Wait for all tasks to complete
void fq_thread_pool_wait_idle(fq_thread_pool_t *pool);

// Check if idle
fq_bool_t fq_thread_pool_is_idle(const fq_thread_pool_t *pool);

// Get thread count
unsigned fq_thread_pool_size(const fq_thread_pool_t *pool);

// Shutdown and cleanup
fq_status_t fq_thread_pool_shutdown(fq_thread_pool_t *pool);
```

## C++ Example

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <vector>

int main()
{
    fq_thread_pool_t *pool = nullptr;
    fq_thread_pool_create_ex(&pool, 4);

    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) data[i] = i;

    auto process = [](void *arg) {
        int *val = static_cast<int *>(arg);
        *val = (*val) * (*val);
    };

    for (auto &val : data) {
        fq_thread_pool_submit_fn(pool, process, &val);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);

    printf("First: %d, Last: %d\n", data[0], data[999]);
    return 0;
}
```

## Error Handling

```c
fq_thread_pool_t *pool = NULL;
fq_status_t st = fq_thread_pool_create_ex(&pool, 4);

if (st != FQ_OK) {
    fprintf(stderr, "Failed to create pool: %s\n", fq_status_string(st));
    return 1;
}

st = fq_thread_pool_submit_fn(pool, my_task, NULL);
if (st != FQ_OK) {
    fprintf(stderr, "Submit failed: %s\n", fq_status_string(st));
}

fq_thread_pool_shutdown(pool);
```
