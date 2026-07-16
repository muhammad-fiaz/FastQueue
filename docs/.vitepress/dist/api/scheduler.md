---
url: /FastQueue/api/scheduler.md
description: >-
  FastQueue scheduler API reference with configuration, submission, and
  lifecycle functions.
---

# Scheduler API

Low-level scheduler API with fine-grained control.

## Types

```c
typedef struct fq_scheduler_t fq_scheduler_t;

typedef struct fq_scheduler_config_t {
    unsigned    thread_count;
    size_t      queue_capacity;
    fq_bool_t   enable_work_stealing;
    fq_bool_t   enable_priority;
    const fq_allocator_t *allocator;
    fq_log_fn   log_callback;
    void       *log_context;
    fq_log_level_t log_level;
} fq_scheduler_config_t;

typedef struct fq_scheduler_stats_t {
    unsigned thread_count;
    size_t   tasks_submitted;
    size_t   tasks_completed;
    size_t   tasks_canceled;
    size_t   tasks_pending;
    size_t   tasks_active;
    uint64_t total_wait_ns;
    uint64_t total_work_ns;
} fq_scheduler_stats_t;
```

## Functions

### Configuration

```c
void fq_scheduler_config_default(fq_scheduler_config_t *config);
```

Initialize config with default values.

### Creation / Destruction

```c
fq_status_t fq_scheduler_create(
    fq_scheduler_t **scheduler,
    const fq_scheduler_config_t *config
);
fq_status_t fq_scheduler_shutdown(fq_scheduler_t *scheduler);
```

### Task Submission

```c
fq_status_t fq_scheduler_submit(fq_scheduler_t *scheduler, fq_task_t *task);
fq_status_t fq_scheduler_submit_fn(fq_scheduler_t *scheduler, fq_task_fn fn, void *user_data);
fq_status_t fq_scheduler_submit_with_future(
    fq_scheduler_t *scheduler, fq_task_t *task, fq_future_t **future
);
```

### Lifecycle

```c
void fq_scheduler_wait_idle(fq_scheduler_t *scheduler);
fq_bool_t fq_scheduler_is_idle(const fq_scheduler_t *scheduler);
fq_bool_t fq_scheduler_is_shutdown(const fq_scheduler_t *scheduler);
void fq_scheduler_cancel_all(fq_scheduler_t *scheduler);
```

`fq_scheduler_cancel_all` forcibly cancels all pending tasks and shuts down the scheduler.

### Statistics

```c
void fq_scheduler_stats(const fq_scheduler_t *scheduler, fq_scheduler_stats_t *stats);
```
