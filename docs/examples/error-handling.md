---
title: Error Handling Example
description: Proper error handling patterns for FastQueue operations.
keywords: error handling, fq_status_t, error codes, example
---

# Error Handling Example

Proper error handling patterns.

## C Version

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

static void safe_task(void *arg)
{
    int id = *(int *)arg;
    printf("Task %d executed safely\n", id);
}

int main(void)
{
    fq_status_t st;

    // 1. Check creation
    fq_thread_pool_t *pool = NULL;
    st = fq_thread_pool_create_ex(&pool, 4);
    if (st != FQ_OK) {
        fprintf(stderr, "Pool creation failed: %s (code=%d)\n",
                fq_status_string(st), st);
        return 1;
    }

    // 2. Check submissions
    for (int i = 0; i < 10; ++i) {
        int *id = malloc(sizeof(int));
        if (!id) {
            fprintf(stderr, "Memory allocation failed\n");
            continue;
        }
        *id = i;

        st = fq_thread_pool_submit_fn(pool, safe_task, id);
        if (st != FQ_OK) {
            fprintf(stderr, "Submit task %d failed: %s\n",
                    i, fq_status_string(st));
            free(id);
        }
    }

    // 3. Wait and check
    fq_thread_pool_wait_idle(pool);

    // 4. Shutdown
    st = fq_thread_pool_shutdown(pool);
    if (st != FQ_OK) {
        fprintf(stderr, "Shutdown failed: %s\n", fq_status_string(st));
        return 1;
    }

    printf("All operations completed successfully.\n");
    return 0;
}
```

## Scheduler with Error Checking

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void validated_task(void *arg)
{
    if (!arg) {
        fprintf(stderr, "Task received NULL argument\n");
        return;
    }
    printf("Result: %d\n", *(int *)arg);
}

int main(void)
{
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 2;

    fq_status_t st = fq_scheduler_create(&scheduler, &cfg);
    if (st != FQ_OK) {
        fprintf(stderr, "Scheduler create: %s\n", fq_status_string(st));
        return 1;
    }

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; ++i) {
        st = fq_scheduler_submit_fn(scheduler, validated_task, &values[i]);
        if (st != FQ_OK) {
            fprintf(stderr, "Submit %d: %s\n", i, fq_status_string(st));
        }
    }

    fq_scheduler_wait_idle(scheduler);

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(scheduler, &stats);
    printf("Stats: submitted=%lu completed=%lu\n",
           stats.tasks_submitted, stats.tasks_completed);

    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

## C++ Version

```cpp
#include <fastqueue/fastqueue.h>
#include <iostream>
#include <memory>

int main()
{
    fq_thread_pool_t *pool = nullptr;
    fq_status_t st = fq_thread_pool_create_ex(&pool, 4);

    if (st != FQ_OK) {
        std::cerr << "Failed: " << fq_status_string(st) << "\n";
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        auto id = std::make_unique<int>(i);
        st = fq_thread_pool_submit_fn(pool, [](void *arg) {
            std::cout << "Task " << *static_cast<int *>(arg) << "\n";
        }, id.get());

        if (st != FQ_OK) {
            std::cerr << "Submit failed: " << fq_status_string(st) << "\n";
        } else {
            id.release(); // Transfer ownership to pool
        }
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Error Codes Reference

| Code | Name | Meaning |
|---|---|---|
| 0 | `FQ_OK` | Success |
| -1 | `FQ_ERR_INVAL` | Invalid argument |
| -2 | `FQ_ERR_NOMEM` | Out of memory |
| -3 | `FQ_ERR_BUSY` | Resource busy |
| -4 | `FQ_ERR_OVERFLOW` | Queue overflow |
| -5 | `FQ_ERR_CLOSED` | Resource closed |
| -6 | `FQ_ERR_TIMEOUT` | Operation timed out |
| -7 | `FQ_ERR_INTERNAL` | Internal error |
