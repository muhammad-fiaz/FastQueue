---
title: Futures Example
description: Complete example of using FastQueue futures for awaitable task results.
keywords: futures, example, awaitable, task result
---

# Futures Example

Use futures to wait for task completion and retrieve results.

## C Version

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

static void compute_square(void *arg)
{
    long *result = (long *)arg;
    *result = (*result) * (*result);
}

static void compute_sum(void *arg)
{
    long *result = (long *)arg;
    long sum = 0;
    for (long i = 1; i <= *result; ++i) sum += i;
    *result = sum;
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;

    fq_status_t st = fq_scheduler_create(&scheduler, &cfg);
    if (st != FQ_OK) {
        fprintf(stderr, "Failed: %s\n", fq_error_string(st));
        return 1;
    }

    // Square computation
    long square_val = 42;
    fq_task_t *square_task = NULL;
    fq_future_t *square_future = NULL;
    fq_task_create(&square_task, compute_square, &square_val, NULL);
    fq_scheduler_submit_with_future(scheduler, square_task, &square_future);

    // Sum computation
    long sum_val = 100;
    fq_task_t *sum_task = NULL;
    fq_future_t *sum_future = NULL;
    fq_task_create(&sum_task, compute_sum, &sum_val, NULL);
    fq_scheduler_submit_with_future(scheduler, sum_task, &sum_future);

    // Wait for results
    fq_future_wait(square_future);
    printf("42 squared = %ld\n", square_val);

    fq_future_wait(sum_future);
    printf("Sum 1..100 = %ld\n", sum_val);

    fq_future_destroy(square_future);
    fq_future_destroy(sum_future);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## C++ Version

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>

int main()
{
    fq_scheduler_t *scheduler = nullptr;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&scheduler, &cfg);

    auto compute_square = [](void *arg) {
        auto *result = static_cast<long *>(arg);
        *result = (*result) * (*result);
    };

    long value = 42;
    fq_task_t *task = nullptr;
    fq_future_t *future = nullptr;
    fq_task_create(&task, compute_square, &value, nullptr);
    fq_scheduler_submit_with_future(scheduler, task, &future);

    fq_future_wait(future);
    std::printf("42 squared = %ld\n", value);

    fq_future_destroy(future);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## Build and Run

```bash
gcc -std=c23 -o futures futures.c -lfastqueue -lpthread
./futures
```

## Expected Output

```
42 squared = 1764
Sum 1..100 = 5050
```
