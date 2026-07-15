# Task API

Header: `<fastqueue/task.h>`

## Types

### fq_task_t

Opaque handle to a unit of work.

## Functions

### fq_task_create

```c
fq_status_t fq_task_create(fq_task_t **task, fq_task_fn callback,
                           void *user_data, const fq_allocator_t *allocator);
```

### fq_task_destroy

```c
void fq_task_destroy(fq_task_t *task);
```

### fq_task_user_data

```c
void *fq_task_user_data(const fq_task_t *task);
```

### fq_task_priority / fq_task_set_priority

```c
fq_priority_t fq_task_priority(const fq_task_t *task);
void fq_task_set_priority(fq_task_t *task, fq_priority_t priority);
```

### fq_task_set_future / fq_task_future

```c
void fq_task_set_future(fq_task_t *task, fq_future_t *future);
fq_future_t *fq_task_future(const fq_task_t *task);
```

### fq_task_is_executed

```c
fq_bool_t fq_task_is_executed(const fq_task_t *task);
```

### fq_task_is_canceled

```c
fq_bool_t fq_task_is_canceled(const fq_task_t *task);
```
