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
void fq_future_wait(fq_future_t *future);
```
Block until the future is ready.

```c
fq_status_t fq_future_wait_timeout(fq_future_t *future, unsigned timeout_ms);
```
Block with timeout. Returns `FQ_OK` on success, `FQ_ERR_TIMEOUT` on timeout.

```c
fq_bool_t fq_future_is_ready(const fq_future_t *future);
```
Non-blocking check if ready.

### Status

```c
fq_status_t fq_future_status(const fq_future_t *future);
```
Get the task's execution status.

```c
fq_status_t fq_future_error(const fq_future_t *future);
```
Get error if task failed.

### Promise (Internal)

```c
void fq_future_set_result(fq_future_t *future, void *result);
void fq_future_set_error(fq_future_t *future, fq_status_t error);
```
