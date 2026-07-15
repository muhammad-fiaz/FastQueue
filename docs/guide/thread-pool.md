# Thread Pool

The thread pool manages a fixed number of worker threads that execute tasks from a shared queue.

## Creating a Pool

```c
// Auto-detect thread count
fq_thread_pool_t *pool = NULL;
fq_thread_pool_create(&pool);

// Explicit thread count
fq_thread_pool_create_ex(&pool, 8);

// Full configuration
fq_scheduler_config_t config;
fq_scheduler_config_default(&config);
config.thread_count = 4;
config.enable_work_stealing = FQ_TRUE;
fq_thread_pool_create_configured(&pool, &config);
```

## Submitting Tasks

```c
// Submit a simple function
fq_thread_pool_submit_fn(pool, my_function, user_data);

// Submit a pre-built task
fq_task_t *task = NULL;
fq_task_create(&task, my_function, user_data, NULL);
fq_thread_pool_submit(pool, task);

// Submit and get a future
fq_future_t *future = NULL;
fq_thread_pool_submit_with_future(pool, task, &future);
```

## Waiting

```c
// Wait for all tasks to complete
fq_thread_pool_wait_idle(pool);

// Check if idle
if (fq_thread_pool_is_idle(pool)) {
    // No tasks pending
}
```

## Shutdown

```c
// Graceful: waits for all tasks to complete
fq_thread_pool_shutdown(pool);
```

## Work Stealing

When enabled (default), idle workers steal tasks from busy workers' queues, improving load balancing and throughput.
