---
title: Future API
description: FastQueue future API reference for awaitable task results.
keywords: future api, fq_future, wait, timeout, ready
---

# Future API

Awaitable results for task completion.

## Types

```c
typedef struct fq_future_t fq_future_t;
```

## Functions

### Creation

```c
fq_status_t fq_future_create(fq_future_t **future, const fq_allocator_t *allocator);
void fq_future_destroy(fq_future_t *future);
```

### Waiting

```c
fq_status_t fq_future_wait(fq_future_t *future);
```
Block until the future is ready. Returns the status of the completed operation.

```c
fq_status_t fq_future_wait_timeout(fq_future_t *future, unsigned timeout_ms, fq_status_t *status);
```
Block with timeout. The `status` pointer receives the result status (may be NULL). Returns `FQ_OK` on success, `FQ_ERR_TIMEOUT` on timeout.

```c
fq_bool_t fq_future_is_ready(const fq_future_t *future);
```
Non-blocking check if ready.

```c
fq_bool_t fq_future_is_canceled(const fq_future_t *future);
```
Check whether the future was canceled.

### Status

```c
fq_status_t fq_future_status(const fq_future_t *future);
```
Get the task's execution status. Returns `FQ_ERR_BUSY` if still pending.

### Callbacks

```c
fq_status_t fq_future_on_complete(fq_future_t *future, fq_completion_fn callback, void *user_data);
```
Attach a completion callback. If the future is already fulfilled, the callback is invoked immediately on the calling thread.

### Cancellation

```c
fq_status_t fq_future_cancel(fq_future_t *future);
```
Cancel a pending future. The completion callback (if any) is invoked with `FQ_ERR_CANCELED`. Returns `FQ_OK` on success, `FQ_ERR_CLOSED` if already fulfilled.

### Promise (Internal)

```c
void fq_future_set_result(fq_future_t *future, fq_status_t status);
```
Fulfill a future with a result status. Called internally by the scheduler.
