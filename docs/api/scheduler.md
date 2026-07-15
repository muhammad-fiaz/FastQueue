# Scheduler API

Header: `<fastqueue/scheduler.h>`

## Types

### fq_scheduler_t

Opaque handle to a job scheduler.

### fq_scheduler_config_t

```c
typedef struct fq_scheduler_config_t {
    unsigned        thread_count;
    size_t          queue_capacity;
    fq_bool_t       enable_work_stealing;
    fq_bool_t       enable_priority;
    const fq_allocator_t *allocator;
    fq_log_fn       log_callback;
    void           *log_context;
    fq_log_level_t  log_level;
} fq_scheduler_config_t;
```

### fq_scheduler_stats_t

```c
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

### fq_scheduler_create

```c
fq_status_t fq_scheduler_create(fq_scheduler_t **scheduler,
                                const fq_scheduler_config_t *config);
```

Create and start a scheduler.

### fq_scheduler_shutdown

```c
fq_status_t fq_scheduler_shutdown(fq_scheduler_t *scheduler);
```

Gracefully shut down.

### fq_scheduler_submit_fn

```c
fq_status_t fq_scheduler_submit_fn(fq_scheduler_t *scheduler,
                                   fq_task_fn fn, void *user_data);
```

Submit a function as a task.

### fq_scheduler_submit_with_future

```c
fq_status_t fq_scheduler_submit_with_future(fq_scheduler_t *scheduler,
                                            fq_task_t *task,
                                            fq_future_t **future);
```

Submit and get a future.

### fq_scheduler_wait_idle

```c
void fq_scheduler_wait_idle(fq_scheduler_t *scheduler);
```

Wait for all tasks to complete.

### fq_scheduler_stats

```c
void fq_scheduler_stats(const fq_scheduler_t *scheduler,
                        fq_scheduler_stats_t *stats);
```

Retrieve runtime statistics.
