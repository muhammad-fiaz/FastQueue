---
title: Graceful Shutdown Example
description: Properly shut down a FastQueue scheduler with graceful task completion.
keywords: graceful shutdown, cleanup, lifecycle, example
---

# Graceful Shutdown Example

Properly shut down a scheduler after all tasks complete.

## C Version

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static fq_scheduler_t *g_scheduler = NULL;

static void signal_handler(int sig)
{
    (void)sig;
    printf("\nCaught signal, shutting down...\n");
    if (g_scheduler) {
        fq_scheduler_cancel_all(g_scheduler);
    }
}

static void long_task(void *arg)
{
    int id = *(int *)arg;
    printf("Task %d started\n", id);
    // Simulate work
    for (volatile int i = 0; i < 1000000; ++i) {}
    printf("Task %d completed\n", id);
}

int main(void)
{
    signal(SIGINT, signal_handler);

    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;

    fq_scheduler_create(&g_scheduler, &cfg);

    for (int i = 0; i < 20; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        fq_scheduler_submit_fn(g_scheduler, long_task, id);
    }

    printf("Waiting for tasks to complete...\n");
    fq_scheduler_wait_idle(g_scheduler);

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(g_scheduler, &stats);
    printf("Completed: %d tasks\n", stats.tasks_completed);

    fq_scheduler_shutdown(g_scheduler);
    g_scheduler = NULL;

    printf("Shutdown complete.\n");
    return 0;
}
```

## C++ Version

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <csignal>

static fq_scheduler_t *g_scheduler = nullptr;
static std::atomic<bool> g_running{true};

static void signal_handler(int) {
    g_running = false;
    if (g_scheduler) fq_scheduler_cancel_all(g_scheduler);
}

int main()
{
    std::signal(SIGINT, signal_handler);

    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&g_scheduler, &cfg);

    for (int i = 0; i < 20; ++i) {
        int *id = new int(i);
        fq_scheduler_submit_fn(g_scheduler, [](void *arg) {
            int id = *static_cast<int *>(arg);
            std::printf("Task %d\n", id);
            for (volatile int j = 0; j < 1000000; ++j) {}
        }, id);
    }

    fq_scheduler_wait_idle(g_scheduler);

    fq_scheduler_stats_t stats;
    fq_scheduler_stats(g_scheduler, &stats);
    std::printf("Completed: %d tasks\n", stats.tasks_completed);

    fq_scheduler_shutdown(g_scheduler);
    g_scheduler = nullptr;
    return 0;
}
```

## Build and Run

```bash
gcc -std=c23 -o graceful_shutdown graceful_shutdown.c -lfastqueue -lpthread
./graceful_shutdown

# Press Ctrl+C to test signal handling
```
