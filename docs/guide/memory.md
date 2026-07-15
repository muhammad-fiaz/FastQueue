# Memory Management

FastQueue supports pluggable allocators for memory-constrained environments.

## Default Allocator

The default allocator uses standard `malloc`/`calloc`/`realloc`/`free`:

```c
const fq_allocator_t *alloc = fq_default_allocator();
void *p = fq_alloc(alloc, 1024);
fq_free(alloc, p);
```

## Custom Allocator

```c
static void *my_alloc(size_t size, void *ctx) {
    return my_custom_malloc(size, ctx);
}

static void *my_realloc(void *ptr, size_t size, void *ctx) {
    return my_custom_realloc(ptr, size, ctx);
}

static void my_free(void *ptr, void *ctx) {
    my_custom_free(ptr, ctx);
}

fq_allocator_t alloc = {
    .alloc   = my_alloc,
    .realloc = my_realloc,
    .free    = my_free,
    .ctx     = &my_context,
};

// Pass to any create function
fq_scheduler_config_t config;
config.allocator = &alloc;
```

## Statistics

```c
fq_mem_stats_t stats;
fq_mem_stats(&stats);

printf("Currently allocated: %zu bytes\n", stats.allocated_bytes);
printf("Peak usage:          %zu bytes\n", stats.peak_bytes);
printf("Total allocations:   %zu\n", stats.total_allocs);
```
