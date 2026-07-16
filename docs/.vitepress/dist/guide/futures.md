---
url: /FastQueue/guide/futures.md
description: >-
  Learn how to use FastQueue futures for awaitable task results with
  spin-then-wait strategy.
---

# Futures

Futures allow you to wait for task completion and retrieve results.

## Basic Usage

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void compute(void *arg)
{
    long *result = (long *)arg;
    long sum = 0;
    for (long i = 0; i < 1000000; ++i) sum += i;
    *result = sum;
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&scheduler, &cfg);

    long result = 0;
    fq_future_t *future = NULL;
    fq_task_t *task = NULL;

    fq_task_create(&task, compute, &result, NULL);
    fq_scheduler_submit_with_future(scheduler, task, &future);

    fq_future_wait(future);
    printf("Result: %ld\n", result);

    fq_future_destroy(future);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## API Reference

### Creation

```c
fq_status_t fq_future_create(fq_future_t **future, const fq_allocator_t *allocator);
void fq_future_destroy(fq_future_t *future);
```

### Waiting

```c
fq_status_t fq_future_wait(fq_future_t *future);
fq_status_t fq_future_wait_timeout(fq_future_t *future, unsigned timeout_ms, fq_status_t *status);
fq_bool_t fq_future_is_ready(const fq_future_t *future);
```

### Error Handling

```c
fq_status_t fq_future_status(const fq_future_t *future);
```

## Multiple Futures

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void compute(void *arg)
{
    long *result = (long *)arg;
    *result = (*result) * (*result);
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&scheduler, &cfg);

    long results[5] = {1, 2, 3, 4, 5};
    fq_future_t *futures[5];

    for (int i = 0; i < 5; ++i) {
        fq_task_t *task = NULL;
        fq_task_create(&task, compute, &results[i], NULL);
        fq_scheduler_submit_with_future(scheduler, task, &futures[i]);
    }

    for (int i = 0; i < 5; ++i) {
        fq_future_wait(futures[i]);
        printf("Result %d: %ld\n", i, results[i]);
        fq_future_destroy(futures[i]);
    }

    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## Timeout

```c
fq_future_t *future = NULL;
fq_task_t *task = NULL;
fq_task_create(&task, slow_task, NULL, NULL);
fq_scheduler_submit_with_future(scheduler, task, &future);

fq_status_t st = fq_future_wait_timeout(future, 5000, NULL); // 5 second timeout
if (st == FQ_OK) {
    printf("Task completed\n");
} else {
    printf("Timeout or error\n");
}

fq_future_destroy(future);
```
