---
title: Scheduler
description: Learn about the FastQueue job scheduler with work stealing, priority support, and statistics.
keywords: scheduler, work stealing, priority, job scheduler, fq_scheduler
---

# Scheduler

The scheduler is the low-level engine behind the thread pool, providing fine-grained control over task execution.

## Basic Usage

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void job(void *arg)
{
    printf("Job %d executed\n", *(int *)arg);
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;

    fq_scheduler_create(&scheduler, &cfg);

    for (int i = 0; i < 100; ++i) {
        int *val = malloc(sizeof(int));
        *val = i;
        fq_scheduler_submit_fn(scheduler, job, val);
    }

    fq_scheduler_wait_idle(scheduler);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## Configuration

```c
fq_scheduler_config_t cfg;
fq_scheduler_config_default(&cfg);

cfg.thread_count         = 8;
cfg.queue_capacity       = 8192;
cfg.enable_work_stealing = FQ_TRUE;
cfg.enable_priority      = FQ_FALSE;
cfg.log_level            = FQ_LOG_INFO;
cfg.log_callback         = my_log_fn;
cfg.log_context          = NULL;
cfg.allocator            = &my_allocator;
```

## API Reference

### Configuration

```c
void fq_scheduler_config_default(fq_scheduler_config_t *config);
```

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

fq_status_t fq_scheduler_submit_fn(
    fq_scheduler_t *scheduler,
    fq_task_fn fn,
    void *user_data
);

fq_status_t fq_scheduler_submit_with_future(
    fq_scheduler_t *scheduler,
    fq_task_t *task,
    fq_future_t **future
);
```

### Lifecycle

```c
void fq_scheduler_wait_idle(fq_scheduler_t *scheduler);
fq_bool_t fq_scheduler_is_idle(const fq_scheduler_t *scheduler);
fq_bool_t fq_scheduler_is_shutdown(const fq_scheduler_t *scheduler);
void fq_scheduler_cancel_all(fq_scheduler_t *scheduler);
```

### Statistics

```c
void fq_scheduler_stats(const fq_scheduler_t *scheduler, fq_scheduler_stats_t *stats);
```

The `fq_scheduler_stats_t` struct contains:
- `thread_count` - Number of worker threads
- `tasks_submitted` - Total tasks submitted
- `tasks_completed` - Total tasks completed
- `tasks_canceled` - Total tasks canceled
- `tasks_pending` - Currently pending tasks
- `tasks_active` - Currently executing tasks
- `total_wait_ns` - Cumulative wait time in nanoseconds
- `total_work_ns` - Cumulative work time in nanoseconds

## Work Stealing

When `enable_work_stealing` is true, idle workers can steal tasks from other workers' queues. This ensures optimal load balancing across all threads.

```c
cfg.enable_work_stealing = FQ_TRUE;
```

## C++ Example

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <thread>
#include <vector>

int main()
{
    fq_scheduler_t *scheduler = nullptr;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&scheduler, &cfg);

    std::vector<int> data(500);
    for (int i = 0; i < 500; ++i) data[i] = i;

    auto compute = [](void *arg) {
        int *val = static_cast<int *>(arg);
        *val = (*val) * 2;
    };

    for (auto &val : data) {
        fq_scheduler_submit_fn(scheduler, compute, &val);
    }

    fq_scheduler_wait_idle(scheduler);

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(scheduler, &stats);
    printf("Completed: %d tasks\n", stats.tasks_completed);

    fq_scheduler_shutdown(scheduler);
    return 0;
}
```
