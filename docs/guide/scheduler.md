# Scheduler

The scheduler is the core engine that distributes tasks across worker threads.

## Configuration

```c
fq_scheduler_config_t config;
fq_scheduler_config_default(&config);

config.thread_count = 4;
config.queue_capacity = 2048;
config.enable_work_stealing = FQ_TRUE;
config.enable_priority = FQ_FALSE;
config.log_callback = my_log_fn;
config.log_level = FQ_LOG_INFO;
```

## Lifecycle

```c
fq_scheduler_t *scheduler = NULL;
fq_scheduler_create(&scheduler, &config);

// Submit work...
fq_scheduler_wait_idle(scheduler);

// Graceful shutdown
fq_scheduler_shutdown(scheduler);
```

## Statistics

```c
fq_scheduler_stats_t stats;
fq_scheduler_stats(scheduler, &stats);

printf("Threads:      %u\n", stats.thread_count);
printf("Submitted:    %zu\n", stats.tasks_submitted);
printf("Completed:    %zu\n", stats.tasks_completed);
printf("Pending:      %zu\n", stats.tasks_pending);
printf("Active:       %zu\n", stats.tasks_active);
```
