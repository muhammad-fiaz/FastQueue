# Quick Start

## Minimal Example

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void my_task(void *arg)
{
    int *value = (int *)arg;
    printf("Processing: %d\n", *value);
}

int main(void)
{
    // Create a thread pool with 4 workers
    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, 4);

    // Submit 100 tasks
    for (int i = 0; i < 100; ++i) {
        int *val = malloc(sizeof(int));
        *val = i;
        fq_thread_pool_submit_fn(pool, my_task, val);
    }

    // Wait for completion
    fq_thread_pool_wait_idle(pool);

    // Clean up
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Using Futures

```c
#include <fastqueue/fastqueue.h>

static void compute(void *arg)
{
    int *val = (int *)arg;
    *val = (*val) * (*val);
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_create(&scheduler, NULL);

    int value = 42;
    fq_task_t *task = NULL;
    fq_task_create(&task, compute, &value, NULL);

    fq_future_t *future = NULL;
    fq_scheduler_submit_with_future(scheduler, task, &future);

    // Block until result is ready
    fq_future_wait(future);
    printf("Result: %d\n", value);  // 1764

    fq_future_destroy(future);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```
