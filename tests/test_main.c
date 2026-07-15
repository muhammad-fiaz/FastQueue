/**
 * @file test_main.c
 * @brief Minimal test runner for FastQueue.
 *
 * A self-contained test harness that does not depend on any external
 * framework.  Each test function returns 0 on success, non-zero on failure.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 

extern int test_queue_basic(void);
extern int test_queue_concurrent(void);
extern int test_task_create_destroy(void);
extern int test_task_execution(void);
extern int test_future_wait(void);
extern int test_future_timeout(void);
extern int test_future_cancel(void);
extern int test_scheduler_basic(void);
extern int test_scheduler_stress(void);
extern int test_thread_pool_basic(void);
extern int test_memory_alloc(void);
extern int test_errors(void);
extern int test_stress_many_jobs(void);

 

typedef struct test_entry_t {
    const char *name;
    int       (*func)(void);
} test_entry_t;

static test_entry_t g_tests[] = {
    { "queue_basic",        test_queue_basic        },
    { "queue_concurrent",   test_queue_concurrent   },
    { "task_create_destroy",test_task_create_destroy },
    { "task_execution",     test_task_execution     },
    { "future_wait",        test_future_wait        },
    { "future_timeout",     test_future_timeout     },
    { "future_cancel",      test_future_cancel      },
    { "scheduler_basic",    test_scheduler_basic    },
    { "scheduler_stress",   test_scheduler_stress   },
    { "thread_pool_basic",  test_thread_pool_basic  },
    { "memory_alloc",       test_memory_alloc       },
    { "errors",             test_errors             },
    { "stress_many_jobs",   test_stress_many_jobs   },
    { NULL, NULL }
};

 

int main(void)
{
    int passed = 0;
    int failed = 0;
    int total  = 0;

    printf("FastQueue v%s  —  test suite\n", fq_version_string());
    printf("═══════════════════════════════════════════════════\n\n");

    for (test_entry_t *t = g_tests; t->name; ++t) {
        total++;
        printf("  %-30s ", t->name);
        fflush(stdout);

        int rc = t->func();
        if (rc == 0) {
            printf("[PASS]\n");
            passed++;
        } else {
            printf("[FAIL] (rc=%d)\n", rc);
            failed++;
        }
    }

    printf("\n═══════════════════════════════════════════════════\n");
    printf("  %d passed, %d failed, %d total\n", passed, failed, total);

    return failed > 0 ? 1 : 0;
}
