---
title: API Overview
description: Complete API reference for FastQueue C23 thread pool and job system library.
keywords: api, reference, fastqueue api, c23 api
---

# API Overview

FastQueue provides a layered API:

## Headers

| Header | Description |
|---|---|
| `fastqueue/fastqueue.h` | Master include (includes everything) |
| `fastqueue/thread_pool.h` | High-level thread pool API |
| `fastqueue/scheduler.h` | Low-level scheduler API |
| `fastqueue/queue.h` | Thread-safe queue |
| `fastqueue/future.h` | Futures and promises |
| `fastqueue/task.h` | Task creation and management |
| `fastqueue/memory.h` | Custom allocator support |
| `fastqueue/errors.h` | Error codes and messages |
| `fastqueue/config.h` | Configuration constants |
| `fastqueue/version.h` | Version information |
| `fastqueue/atomic.h` | Atomic operations |
| `fastqueue/platform.h` | Platform abstraction |
| `fastqueue/parallel.h` | Parallel for-loop utility |
| `fastqueue/time.h` | Time and timer utilities |
| `fastqueue/types.h` | Type definitions |

## Naming Conventions

- All public symbols prefixed with `fq_`
- Types: `fq_<name>_t`
- Functions: `fq_<module>_<action>`
- Enums: `fq_<name>_t` with `FQ_<NAME>_<VALUE>`
- Constants: `FQ_<NAME>`

## Error Handling

All functions return `fq_status_t`:

```c
fq_status_t st = fq_thread_pool_create_ex(&pool, 4);
if (st != FQ_OK) {
    fprintf(stderr, "Error: %s\n", fq_status_string(st));
    return 1;
}
```

## Quick Reference

### Thread Pool
```c
fq_thread_pool_create_ex(&pool, threads);
fq_thread_pool_submit_fn(pool, fn, data);
fq_thread_pool_wait_idle(pool);
fq_thread_pool_shutdown(pool);
```

### Scheduler
```c
fq_scheduler_create(&scheduler, &cfg);
fq_scheduler_submit_fn(scheduler, fn, data);
fq_scheduler_wait_idle(scheduler);
fq_scheduler_shutdown(scheduler);
```

### Future
```c
fq_future_create(&future, allocator);
fq_future_wait(future);
fq_future_destroy(future);
```

### Queue
```c
fq_queue_create(&queue, capacity, allocator);
fq_queue_push(queue, task);
fq_queue_pop(queue, &task);
fq_queue_destroy(queue);
```

### Parallel For
```c
fq_thread_pool_parallel_for(pool, 0, N, fn, ctx);
fq_scheduler_parallel_for(scheduler, 0, N, fn, ctx);
```

### Time
```c
int64_t now = fq_time_now_ns();
fq_timer_start(&timer);
int64_t elapsed = fq_timer_elapsed_ns(&timer);
```
