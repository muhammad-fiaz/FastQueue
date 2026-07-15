# Thread Pool API

Header: `<fastqueue/thread_pool.h>`

## Types

### fq_thread_pool_t

Opaque handle to a thread pool instance.

## Functions

### fq_thread_pool_create

```c
fq_status_t fq_thread_pool_create(fq_thread_pool_t **pool);
```

Create a thread pool with default settings (auto-detect thread count).

### fq_thread_pool_create_ex

```c
fq_status_t fq_thread_pool_create_ex(fq_thread_pool_t **pool,
                                     unsigned thread_count);
```

Create a thread pool with explicit thread count.

### fq_thread_pool_shutdown

```c
fq_status_t fq_thread_pool_shutdown(fq_thread_pool_t *pool);
```

Gracefully shut down the pool. Waits for all pending tasks.

### fq_thread_pool_submit_fn

```c
fq_status_t fq_thread_pool_submit_fn(fq_thread_pool_t *pool,
                                     fq_task_fn fn, void *user_data);
```

Submit a function as a task.

### fq_thread_pool_wait_idle

```c
void fq_thread_pool_wait_idle(fq_thread_pool_t *pool);
```

Block until no tasks are pending or executing.

### fq_thread_pool_size

```c
unsigned fq_thread_pool_size(const fq_thread_pool_t *pool);
```

Return the number of worker threads.
