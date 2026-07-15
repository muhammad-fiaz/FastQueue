---
title: Queue API
description: FastQueue thread-safe MPMC queue API reference.
keywords: queue api, fq_queue, push, pop, thread safe, mpmc
---

# Queue API

Thread-safe multi-producer multi-consumer queue.

## Types

```c
typedef struct fq_queue_t fq_queue_t;
```

## Functions

### Creation

```c
fq_status_t fq_queue_create(fq_queue_t **queue, size_t capacity, const fq_allocator_t *allocator);
void fq_queue_destroy(fq_queue_t *queue);
```

### Operations

```c
fq_status_t fq_queue_push(fq_queue_t *queue, fq_task_t *task);
fq_status_t fq_queue_pop(fq_queue_t *queue, fq_task_t **task);
fq_status_t fq_queue_try_pop(fq_queue_t *queue, fq_task_t **task);
```

### Queries

```c
size_t fq_queue_size(const fq_queue_t *queue);
fq_bool_t fq_queue_empty(const fq_queue_t *queue);
size_t fq_queue_capacity(const fq_queue_t *queue);
```

### Bulk Operations

```c
size_t fq_queue_drain(fq_queue_t *queue, fq_task_t **out, size_t max);
```
Drain up to `max` tasks from the queue into `out`. Returns the number of tasks actually drained.
