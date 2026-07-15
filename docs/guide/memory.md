---
title: Memory Management
description: Learn about FastQueue custom allocators and memory management for constrained environments.
keywords: memory, allocator, custom allocator, memory management, fq_allocator
---

# Memory Management

FastQueue supports pluggable allocators for memory-constrained environments.

## Default Allocator

By default, FastQueue uses the system `malloc`/`free`:

```c
fq_thread_pool_t *pool = NULL;
fq_thread_pool_create_ex(&pool, 4); // Uses default allocator
```

## Custom Allocator

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

static void *my_alloc(size_t size, void *ctx)
{
    (void)ctx;
    printf("Allocating %zu bytes\n", size);
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

int main(void)
{
    fq_allocator_t alloc = {
        .alloc   = my_alloc,
        .realloc = my_realloc,
        .free    = my_free,
        .ctx     = NULL
    };

    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.allocator = &alloc;
    cfg.thread_count = 4;

    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_create(&scheduler, &cfg);

    // Use scheduler...

    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## Arena Allocator Example

For high-throughput scenarios, an arena allocator can reduce allocation overhead:

```c
#include <fastqueue/fastqueue.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_SIZE (1024 * 1024)

typedef struct {
    char buffer[ARENA_SIZE];
    size_t offset;
} arena_t;

static void *arena_alloc(size_t size, void *ctx)
{
    arena_t *arena = (arena_t *)ctx;
    size_t aligned = (size + 7) & ~(size_t)7;
    if (arena->offset + aligned > ARENA_SIZE) return NULL;
    void *ptr = arena->buffer + arena->offset;
    arena->offset += aligned;
    return ptr;
}

static void arena_free(void *ptr, void *ctx)
{
    (void)ptr;
    (void)ctx;
}

int main(void)
{
    static arena_t arena = {0};

    fq_allocator_t alloc = {
        .alloc   = arena_alloc,
        .free    = arena_free,
        .ctx     = &arena
    };

    fq_thread_pool_t *pool = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.allocator = &alloc;
    cfg.thread_count = 4;

    fq_thread_pool_create_configured(&pool, &cfg);

    for (int i = 0; i < 1000; ++i) {
        int *val = arena_alloc(sizeof(int), &arena);
        if (val) {
            *val = i;
            fq_thread_pool_submit_fn(pool, my_task, val);
        }
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## API Reference

```c
typedef struct fq_allocator_t {
    void *(*alloc)(size_t size, void *ctx);
    void *(*realloc)(void *ptr, size_t size, void *ctx);
    void  (*free)(void *ptr, void *ctx);
    void  *ctx;
} fq_allocator_t;

const fq_allocator_t *fq_default_allocator(void);
```
