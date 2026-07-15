---
title: Task API
description: FastQueue task API reference for creating and managing individual tasks.
keywords: task api, fq_task, create, priority, cancel
---

# Task API

Task creation and management.

## Types

```c
typedef struct fq_task_t fq_task_t;
typedef void (*fq_task_fn)(void *user_data);
typedef void (*fq_completion_fn)(void *user_data, fq_status_t status);
```

## Enums

```c
typedef enum fq_priority_t {
    FQ_PRIORITY_LOW    = 0,
    FQ_PRIORITY_NORMAL = 1,
    FQ_PRIORITY_HIGH   = 2,
    FQ_PRIORITY_URGENT = 3
} fq_priority_t;
```

## Functions

### Creation

```c
fq_status_t fq_task_create(
    fq_task_t **task,
    fq_task_fn fn,
    void *user_data,
    const fq_allocator_t *allocator
);
void fq_task_destroy(fq_task_t *task);
```

### Creation with Completion

```c
fq_status_t fq_task_create_with_completion(
    fq_task_t **task,
    fq_task_fn callback,
    void *user_data,
    fq_completion_fn completion,
    void *completion_data,
    const fq_allocator_t *allocator
);
```
Create a task with an attached completion callback. The completion is invoked with the task's status when execution finishes.

### Properties

```c
void *fq_task_user_data(const fq_task_t *task);
fq_priority_t fq_task_priority(const fq_task_t *task);
void fq_task_set_priority(fq_task_t *task, fq_priority_t priority);
fq_bool_t fq_task_is_executed(const fq_task_t *task);
fq_bool_t fq_task_is_canceled(const fq_task_t *task);
```

### Future

```c
void fq_task_set_future(fq_task_t *task, fq_future_t *future);
fq_future_t *fq_task_future(const fq_task_t *task);
```
