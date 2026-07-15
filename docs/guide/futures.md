# Futures

Futures allow you to await the result of an asynchronous operation.

## Basic Usage

```c
// Create a task
fq_task_t *task = NULL;
fq_task_create(&task, compute_square, &value, NULL);

// Submit and get a future
fq_future_t *future = NULL;
fq_scheduler_submit_with_future(scheduler, task, &future);

// Wait for result
fq_status_t status = fq_future_wait(future);
if (fq_status_ok(status)) {
    printf("Result ready\n");
}
```

## Timeout

```c
fq_status_t status;
if (fq_future_wait_timeout(future, 1000, &status) == FQ_ERR_TIMEOUT) {
    printf("Timed out after 1 second\n");
}
```

## Completion Callback

```c
fq_future_on_complete(future, on_done, user_data);
```

## Cancellation

```c
fq_future_cancel(future);
// Future is now ready with FQ_ERR_CANCELED status
```
