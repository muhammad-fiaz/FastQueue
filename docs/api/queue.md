# Queue API

Header: `<fastqueue/queue.h>`

## Types

### fq_queue_t

Opaque thread-safe FIFO task queue.

## Functions

### fq_queue_create

```c
fq_status_t fq_queue_create(fq_queue_t **queue, size_t capacity,
                            const fq_allocator_t *allocator);
```

Create a queue with given capacity (rounded up to power of 2).

### fq_queue_destroy

```c
void fq_queue_destroy(fq_queue_t *queue);
```

Destroy the queue. Does not free pending tasks.

### fq_queue_push

```c
fq_status_t fq_queue_push(fq_queue_t *queue, fq_task_t *task);
```

Push a task to the back. Returns `FQ_ERR_OVERFLOW` if full.

### fq_queue_pop

```c
fq_status_t fq_queue_pop(fq_queue_t *queue, fq_task_t **task);
```

Pop a task from the front. Returns `FQ_ERR_BUSY` if empty.

### fq_queue_try_pop

```c
fq_status_t fq_queue_try_pop(fq_queue_t *queue, fq_task_t **task);
```

Non-blocking pop.

### fq_queue_size

```c
size_t fq_queue_size(const fq_queue_t *queue);
```

Return current number of tasks.

### fq_queue_empty

```c
fq_bool_t fq_queue_empty(const fq_queue_t *queue);
```

Check if the queue is empty.

### fq_queue_drain

```c
size_t fq_queue_drain(fq_queue_t *queue, fq_task_t **out, size_t max);
```

Atomically remove up to `max` tasks.
