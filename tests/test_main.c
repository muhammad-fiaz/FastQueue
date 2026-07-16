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
extern int test_future_on_complete(void);
extern int test_future_on_complete_already_ready(void);
extern int test_future_on_complete_with_status(void);
extern int test_future_wait_blocking(void);
extern int test_scheduler_basic(void);
extern int test_scheduler_stress(void);
extern int test_thread_pool_basic(void);
extern int test_memory_alloc(void);
extern int test_errors(void);
extern int test_stress_many_jobs(void);
extern int test_scheduler_cancel_all(void);
extern int test_scheduler_is_idle(void);
extern int test_scheduler_is_shutdown(void);
extern int test_scheduler_submit(void);
extern int test_scheduler_submit_with_future(void);
extern int test_scheduler_stats_timing(void);
extern int test_scheduler_priority(void);
extern int test_scheduler_no_work_stealing(void);
extern int test_thread_pool_create(void);
extern int test_thread_pool_create_configured(void);
extern int test_thread_pool_submit(void);
extern int test_thread_pool_submit_with_future(void);
extern int test_thread_pool_is_idle(void);
extern int test_thread_pool_stats(void);
extern int test_scheduler_parallel_for(void);
extern int test_thread_pool_parallel_for(void);
extern int test_time_now_ns(void);
extern int test_timer(void);
extern int test_hardware_concurrency(void);
extern int test_thread_sleep(void);
extern int test_thread_yield(void);

typedef struct test_entry_t {
    const char *name;
    int       (*func)(void);
} test_entry_t;

static test_entry_t g_tests[] = {
    { "queue_basic",                    test_queue_basic                    },
    { "queue_concurrent",               test_queue_concurrent               },
    { "task_create_destroy",            test_task_create_destroy            },
    { "task_execution",                 test_task_execution                 },
    { "future_wait",                    test_future_wait                    },
    { "future_timeout",                 test_future_timeout                 },
    { "future_cancel",                  test_future_cancel                  },
    { "future_on_complete",             test_future_on_complete             },
    { "future_on_complete_already_ready", test_future_on_complete_already_ready },
    { "future_on_complete_with_status", test_future_on_complete_with_status },
    { "future_wait_blocking",           test_future_wait_blocking           },
    { "scheduler_basic",                test_scheduler_basic                },
    { "scheduler_stress",               test_scheduler_stress               },
    { "scheduler_cancel_all",           test_scheduler_cancel_all           },
    { "scheduler_is_idle",              test_scheduler_is_idle              },
    { "scheduler_is_shutdown",          test_scheduler_is_shutdown          },
    { "scheduler_submit",               test_scheduler_submit               },
    { "scheduler_submit_with_future",   test_scheduler_submit_with_future   },
    { "scheduler_stats_timing",         test_scheduler_stats_timing         },
    { "scheduler_priority",             test_scheduler_priority             },
    { "scheduler_no_work_stealing",     test_scheduler_no_work_stealing     },
    { "scheduler_parallel_for",         test_scheduler_parallel_for         },
    { "thread_pool_basic",              test_thread_pool_basic              },
    { "thread_pool_create",             test_thread_pool_create             },
    { "thread_pool_create_configured",  test_thread_pool_create_configured  },
    { "thread_pool_submit",             test_thread_pool_submit             },
    { "thread_pool_submit_with_future", test_thread_pool_submit_with_future },
    { "thread_pool_is_idle",            test_thread_pool_is_idle            },
    { "thread_pool_stats",              test_thread_pool_stats              },
    { "thread_pool_parallel_for",       test_thread_pool_parallel_for       },
    { "memory_alloc",                   test_memory_alloc                   },
    { "errors",                         test_errors                         },
    { "stress_many_jobs",               test_stress_many_jobs               },
    { "time_now_ns",                    test_time_now_ns                    },
    { "timer",                          test_timer                          },
    { "hardware_concurrency",           test_hardware_concurrency           },
    { "thread_sleep",                   test_thread_sleep                   },
    { "thread_yield",                   test_thread_yield                   },
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
        printf("  %-40s ", t->name);
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
