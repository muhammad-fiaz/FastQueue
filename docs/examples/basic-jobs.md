---
title: Basic Jobs Example
description: Complete example of submitting and executing basic jobs with the FastQueue thread pool.
keywords: basic jobs, example, thread pool, task submission
---

# Basic Jobs Example

Submit and execute simple tasks using the thread pool.

## C Version

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

static void print_task(void *arg)
{
    int id = *(int *)arg;
    printf("Task %d executed on thread\n", id);
}

int main(void)
{
    fq_thread_pool_t *pool = NULL;
    fq_status_t st = fq_thread_pool_create_ex(&pool, 4);
    if (st != FQ_OK) {
        fprintf(stderr, "Failed to create pool: %s\n", fq_status_string(st));
        return 1;
    }

    printf("Submitting 8 tasks...\n");

    for (int i = 1; i <= 8; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        st = fq_thread_pool_submit_fn(pool, print_task, id);
        if (st != FQ_OK) {
            fprintf(stderr, "Submit failed: %s\n", fq_status_string(st));
            free(id);
        }
    }

    printf("Waiting for all tasks...\n");
    fq_thread_pool_wait_idle(pool);

    printf("Done.\n");
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## C++ Version

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

int main()
{
    fq_thread_pool_t *pool = nullptr;
    fq_thread_pool_create_ex(&pool, 4);

    auto print_task = [](void *arg) {
        int id = *static_cast<int *>(arg);
        std::printf("Task %d executed on thread\n", id);
    };

    std::printf("Submitting 8 tasks...\n");

    for (int i = 1; i <= 8; ++i) {
        int *id = new int(i);
        fq_thread_pool_submit_fn(pool, print_task, id);
    }

    std::printf("Waiting for all tasks...\n");
    fq_thread_pool_wait_idle(pool);

    std::printf("Done.\n");
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Build and Run

```bash
# C
gcc -std=c23 -o basic_jobs basic_jobs.c -lfastqueue -lpthread
./basic_jobs

# C++
g++ -std=c++20 -o basic_jobs basic_jobs.cpp -lfastqueue -lpthread
./basic_jobs
```

## Expected Output

```
Submitting 8 tasks...
Waiting for all tasks...
Task 1 executed on thread
Task 5 executed on thread
Task 2 executed on thread
Task 6 executed on thread
Task 3 executed on thread
Task 7 executed on thread
Task 4 executed on thread
Task 8 executed on thread
Done.
```
