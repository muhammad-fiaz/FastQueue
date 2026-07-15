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
```

## Enums

```c
typedef enum fq_priority_t {
    FQ_PRIORITY_LOW    = 0,
    FQ_PRIORITY_NORMAL = 1,
    FQ_PRIORITY_HIGH   = 2
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

### Execution

```c
void fq_task_execute(fq_task_t *task);
```
Execute the task's function. Called by the scheduler.

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

### Cancellation

```c
void fq_task_cancel(fq_task_t *task);
```
