---
title: Troubleshooting
description: Common issues and solutions for FastQueue thread pool library.
keywords: troubleshooting, debug, issues, errors, help
---

# Troubleshooting

## Build Issues

### MSVC: C4114 warnings

If you see volatile qualifier warnings, ensure you're using the latest FastQueue headers which use proper `volatile` qualifiers for MSVC atomics.

### GCC/Clang: unknown type name '_Thread_local'

Ensure you're compiling with C11 or later support (recommended):
```bash
gcc -std=c11 ...
```

### CMake: Could not find package

If using FetchContent, ensure the git tag is correct:
```cmake
FetchContent_Declare(
    FastQueue
    GIT_REPOSITORY https://github.com/muhammad-fiaz/FastQueue.git
    GIT_TAG main
)
```

## Runtime Issues

### Deadlock in wait_idle

If `fq_scheduler_wait_idle` hangs:

1. Check that all tasks complete (no infinite loops)
2. Ensure tasks don't call `wait_idle` recursively
3. Verify queue capacity is sufficient:
```c
cfg.queue_capacity = 8192; // Increase capacity
```

### Tasks not executing

1. Check that the scheduler is not shut down
2. Verify task submission succeeded:
```c
fq_status_t st = fq_scheduler_submit_fn(scheduler, task, data);
if (st != FQ_OK) {
    fprintf(stderr, "Submit failed: %s\n", fq_status_string(st));
}
```

3. Check thread count:
```c
printf("Thread count: %u\n", cfg.thread_count);
```

### High memory usage

1. Reduce queue capacity
2. Use custom allocator with arena/pool strategy
3. Ensure tasks don't accumulate:
```c
fq_scheduler_wait_idle(scheduler); // Wait before submitting more
```

### Tasks appear lost

1. Check queue capacity - tasks submitted to full queues are dropped
2. Use `fq_scheduler_stats` to verify:
```c
fq_scheduler_stats_t stats;
fq_scheduler_stats(scheduler, &stats);
printf("Submitted: %d, Completed: %d\n", stats.tasks_submitted, stats.tasks_completed);
```

## Sanitizer Reports

### AddressSanitizer: heap-use-after-free

Ensure tasks don't access freed memory:
```c
int *data = malloc(sizeof(int));
*data = 42;
fq_scheduler_submit_fn(scheduler, task, data);
// Don't free data here - let the task do it
```

### ThreadSanitizer: data race

Ensure shared data is properly synchronized:
```c
// Use mutex for shared data
fq_mutex_t mutex;
fq_mutex_init(&mutex);

static void safe_task(void *arg) {
    fq_mutex_lock(&mutex);
    shared_counter++;
    fq_mutex_unlock(&mutex);
}
```

## Getting Help

- [GitHub Issues](https://github.com/muhammad-fiaz/FastQueue/issues)
- [API Reference](/api/overview)
