---
title: Custom Allocator Example
description: Use a custom allocator with FastQueue for memory-constrained environments.
keywords: custom allocator, memory management, arena allocator, example
---

# Custom Allocator Example

Use a custom allocator with FastQueue.

## C Version

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

static int alloc_count = 0;
static int free_count = 0;

static void *counting_alloc(size_t size, void *ctx)
{
    (void)ctx;
    alloc_count++;
    return malloc(size);
}

static void counting_free(void *ptr, void *ctx)
{
    (void)ctx;
    free_count++;
    free(ptr);
}

static void *counting_realloc(void *ptr, size_t new_size, void *ctx)
{
    (void)ctx;
    return realloc(ptr, new_size);
}

static void task_fn(void *arg)
{
    printf("Task %d executed\n", *(int *)arg);
}

int main(void)
{
    fq_allocator_t alloc = {
        .alloc   = counting_alloc,
        .realloc = counting_realloc,
        .free    = counting_free,
        .ctx     = NULL
    };

    fq_thread_pool_t *pool = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.allocator = &alloc;
    cfg.thread_count = 4;

    fq_thread_pool_create_configured(&pool, &cfg);

    for (int i = 0; i < 50; ++i) {
        int *val = malloc(sizeof(int));
        *val = i;
        fq_thread_pool_submit_fn(pool, task_fn, val);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);

    printf("Allocations: %d, Frees: %d\n", alloc_count, free_count);
    return 0;
}
```

## Arena Allocator

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <string.h>

#define ARENA_SIZE (1024 * 1024)

typedef struct {
    char buffer[ARENA_SIZE];
    size_t offset;
} arena_t;

static void *arena_alloc(size_t size, void *ctx)
{
    arena_t *a = (arena_t *)ctx;
    size_t aligned = (size + 7) & ~(size_t)7;
    if (a->offset + aligned > ARENA_SIZE) return NULL;
    void *ptr = a->buffer + a->offset;
    a->offset += aligned;
    return ptr;
}

static void arena_free(void *ptr, void *ctx)
{
    (void)ptr; (void)ctx;
}

static void *arena_realloc(void *ptr, size_t new_size, void *ctx)
{
    (void)ptr; (void)ctx;
    return NULL;
}

int main(void)
{
    static arena_t arena = {0};

    fq_allocator_t alloc = {
        .alloc   = arena_alloc,
        .realloc = arena_realloc,
        .free    = arena_free,
        .ctx     = &arena
    };

    fq_thread_pool_t *pool = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.allocator = &alloc;
    cfg.thread_count = 4;

    fq_thread_pool_create_configured(&pool, &cfg);

    for (int i = 0; i < 100; ++i) {
        int *val = arena_alloc(sizeof(int), &arena);
        if (val) {
            *val = i;
            fq_thread_pool_submit_fn(pool, task_fn, val);
        }
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## C++ Version

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <cstdlib>

static int alloc_count = 0;

static void *tracking_alloc(std::size_t size, void *) {
    alloc_count++;
    return std::malloc(size);
}

static void tracking_free(void *ptr, void *) {
    alloc_count--;
    std::free(ptr);
}

static void *tracking_realloc(void *ptr, size_t new_size, void *) {
    return std::realloc(ptr, new_size);
}

int main()
{
    fq_allocator_t alloc = { tracking_alloc, tracking_realloc, tracking_free, nullptr };

    fq_thread_pool_t *pool = nullptr;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.allocator = &alloc;
    cfg.thread_count = 4;

    fq_thread_pool_create_configured(&pool, &cfg);

    for (int i = 0; i < 50; ++i) {
        int *val = new int(i);
        fq_thread_pool_submit_fn(pool, [](void *arg) {
            std::printf("Task %d\n", *static_cast<int *>(arg));
        }, val);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);

    std::printf("Active allocations: %d\n", alloc_count);
    return 0;
}
```

## Expected Output

```
Task 0 executed
Task 1 executed
...
Allocations: 51, Frees: 51
```
