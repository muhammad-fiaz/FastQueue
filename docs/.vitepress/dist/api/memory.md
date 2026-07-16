---
url: /FastQueue/api/memory.md
description: FastQueue memory allocator API reference for custom memory management.
---

# Memory API

Custom allocator interface and memory utilities.

## Types

```c
typedef struct fq_allocator_t {
    void *(*alloc)(size_t size, void *ctx);
    void *(*realloc)(void *ptr, size_t size, void *ctx);
    void  (*free)(void *ptr, void *ctx);
    void  *ctx;
} fq_allocator_t;
```

## Allocator Functions

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

## Alignment Utilities

```c
size_t fq_align_up(size_t size, size_t alignment);
```

Round `size` up to the nearest multiple of `alignment` (must be a power of 2).

```c
fq_bool_t fq_is_aligned(const void *ptr, size_t alignment);
```

Check whether `ptr` is aligned to `alignment`.

## Memory Statistics

```c
typedef struct fq_mem_stats_t {
    size_t allocated_bytes;
    size_t total_allocs;
    size_t total_frees;
    size_t peak_bytes;
} fq_mem_stats_t;

void fq_mem_stats(fq_mem_stats_t *stats);
```

Retrieve global memory allocation statistics.

## Example

```c
fq_allocator_t alloc = {
    .alloc   = my_alloc,
    .realloc = my_realloc,
    .free    = my_free,
    .ctx     = my_ctx
};

fq_scheduler_config_t cfg;
fq_scheduler_config_default(&cfg);
cfg.allocator = &alloc;
```
