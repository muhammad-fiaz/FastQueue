# Memory API

Header: `<fastqueue/memory.h>`

## Types

### fq_allocator_t

```c
typedef struct fq_allocator_t {
    void *(*alloc)(size_t size, void *ctx);
    void *(*realloc)(void *ptr, size_t size, void *ctx);
    void  (*free)(void *ptr, void *ctx);
    void  *ctx;
} fq_allocator_t;
```

### fq_mem_stats_t

```c
typedef struct fq_mem_stats_t {
    size_t allocated_bytes;
    size_t total_allocs;
    size_t total_frees;
    size_t peak_bytes;
} fq_mem_stats_t;
```

## Functions

### fq_default_allocator

```c
const fq_allocator_t *fq_default_allocator(void);
```

### fq_alloc / fq_calloc / fq_realloc / fq_free

```c
void *fq_alloc(const fq_allocator_t *alloc, size_t size);
void *fq_calloc(const fq_allocator_t *alloc, size_t count, size_t size);
void *fq_realloc(const fq_allocator_t *alloc, void *ptr, size_t size);
void fq_free(const fq_allocator_t *alloc, void *ptr);
```

### fq_align_up / fq_is_aligned

```c
size_t fq_align_up(size_t size, size_t alignment);
fq_bool_t fq_is_aligned(const void *ptr, size_t alignment);
```

### fq_mem_stats

```c
void fq_mem_stats(fq_mem_stats_t *stats);
```
