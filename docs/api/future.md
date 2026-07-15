# Future API

Header: `<fastqueue/future.h>`

## Types

### fq_future_t

Opaque handle to an awaitable result.

## Functions

### fq_future_create

```c
fq_status_t fq_future_create(fq_future_t **future,
                             const fq_allocator_t *allocator);
```

### fq_future_destroy

```c
void fq_future_destroy(fq_future_t *future);
```

### fq_future_wait

```c
fq_status_t fq_future_wait(fq_future_t *future);
```

Block until fulfilled. Returns the result status.

### fq_future_wait_timeout

```c
fq_status_t fq_future_wait_timeout(fq_future_t *future,
                                   unsigned timeout_ms,
                                   fq_status_t *status);
```

Wait with timeout. Returns `FQ_ERR_TIMEOUT` on expiry.

### fq_future_is_ready

```c
fq_bool_t fq_future_is_ready(const fq_future_t *future);
```

### fq_future_status

```c
fq_status_t fq_future_status(const fq_future_t *future);
```

### fq_future_cancel

```c
fq_status_t fq_future_cancel(fq_future_t *future);
```

Cancel a pending future.

### fq_future_on_complete

```c
fq_status_t fq_future_on_complete(fq_future_t *future,
                                  fq_completion_fn callback,
                                  void *user_data);
```

Attach a completion callback.
