---
url: /FastQueue/guide/quick-start.md
description: >-
  Write your first FastQueue program in minutes. Learn thread pools, job
  submission, and futures with complete C and C++ examples.
---

# Quick Start

## Your First Thread Pool

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void print_task(void *arg)
{
    int id = *(int *)arg;
    printf("Task %d executed on thread\n", id);
}

int main(void)
{
    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, 4);

    for (int i = 0; i < 100; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        fq_thread_pool_submit_fn(pool, print_task, id);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

Compile with:

```bash
gcc -std=c23 -o myapp main.c -lfastqueue -lpthread
```

## Using the Scheduler

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void compute(void *arg)
{
    int n = *(int *)arg;
    long sum = 0;
    for (int i = 0; i <= n; ++i) sum += i;
    printf("Sum 1..%d = %ld\n", n, sum);
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;

    fq_scheduler_create(&scheduler, &cfg);

    for (int i = 1; i <= 20; ++i) {
        int *val = malloc(sizeof(int));
        *val = i * 1000;
        fq_scheduler_submit_fn(scheduler, compute, val);
    }

    fq_scheduler_wait_idle(scheduler);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## Using Futures

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void heavy_computation(void *arg)
{
    long *result = (long *)arg;
    long sum = 0;
    for (long i = 0; i < 1000000; ++i) sum += i;
    *result = sum;
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&scheduler, &cfg);

    long result = 0;
    fq_future_t *future = NULL;
    fq_task_t *task = NULL;

    fq_task_create(&task, heavy_computation, &result, NULL);
    fq_scheduler_submit_with_future(scheduler, task, &future);

    fq_future_wait(future);

    printf("Result: %ld\n", result);

    fq_future_destroy(future);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## C++ Usage

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <cstdlib>

int main()
{
    fq_thread_pool_t *pool = nullptr;
    fq_thread_pool_create_ex(&pool, 4);

    auto task_fn = [](void *arg) {
        int id = *static_cast<int *>(arg);
        std::printf("Task %d executed\n", id);
    };

    for (int i = 0; i < 50; ++i) {
        int *id = new int(i);
        fq_thread_pool_submit_fn(pool, task_fn, id);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Thread Pool with Custom Allocator

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

static void *my_alloc(size_t size, void *ctx)
{
    (void)ctx;
    return malloc(size);
}

static void my_free(void *ptr, void *ctx)
{
    (void)ctx;
    free(ptr);
}

static void *my_realloc(void *ptr, size_t new_size, void *ctx)
{
    (void)ctx;
    return realloc(ptr, new_size);
}

static void task_fn(void *arg)
{
    printf("Task with custom allocator: %d\n", *(int *)arg);
}

int main(void)
{
    fq_allocator_t alloc = { my_alloc, my_realloc, my_free, NULL };

    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, 4);

    for (int i = 0; i < 10; ++i) {
        int *val = malloc(sizeof(int));
        *val = i;
        fq_thread_pool_submit_fn(pool, task_fn, val);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Next Steps

* [Thread Pool](/guide/thread-pool) - Deep dive into thread pool configuration
* [Scheduler](/guide/scheduler) - Priority scheduling and work stealing
* [Futures](/guide/futures) - Awaitable results
* [API Reference](/api/overview) - Complete API documentation
