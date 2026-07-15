---
title: Memory API
description: FastQueue memory allocator API reference for custom memory management.
keywords: memory api, fq_allocator, custom allocator, memory management
---

# Memory API

Custom allocator interface.

## Types

```c
typedef struct fq_allocator_t {
    void *(*alloc)(size_t size, void *context);
    void  (*free)(void *ptr, void *context);
    void *(*calloc)(size_t count, size_t size, void *context);
    void *(*realloc)(void *ptr, size_t new_size, void *context);
    void *context;
} fq_allocator_t;
```

## Functions

```c
const fq_allocator_t *fq_default_allocator(void);
```
Get the default system allocator.

```c
void *fq_alloc(const fq_allocator_t *allocator, size_t size);
void *fq_calloc(const fq_allocator_t *allocator, size_t count, size_t size);
void *fq_realloc(const fq_allocator_t *allocator, void *ptr, size_t new_size);
void fq_free(const fq_allocator_t *allocator, void *ptr);
```
Allocate memory using the provided allocator.

## Example

```c
fq_allocator_t alloc = {
    .alloc   = my_alloc,
    .free    = my_free,
    .context = my_ctx
};

fq_scheduler_config_t cfg;
fq_scheduler_config_default(&cfg);
cfg.allocator = &alloc;
```
