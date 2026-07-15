/**
 * @file test_queue.c
 * @brief Queue unit tests.
 */

#include "fastqueue/fastqueue.h"

#include <stdio.h>

 

int test_queue_basic(void)
{
    fq_queue_t *q = NULL;
    fq_status_t st = fq_queue_create(&q, 16, NULL);
    if (st != FQ_OK) return 1;

    if (fq_queue_capacity(q) < 15) { fq_queue_destroy(q); return 2; }
    if (!fq_queue_empty(q))         { fq_queue_destroy(q); return 3; }

    /* Create dummy tasks. */
    fq_task_t *t1 = NULL, *t2 = NULL, *t3 = NULL;
    fq_task_fn noop = (fq_task_fn)(void (*)(void))NULL;
    /* We need a valid function pointer. */
    static int sink_var = 0;
    (void)sink_var;

    st = fq_task_create(&t1, (fq_task_fn)1, (void *)1, NULL);
    if (st != FQ_OK) { fq_queue_destroy(q); return 4; }
    st = fq_task_create(&t2, (fq_task_fn)2, (void *)2, NULL);
    if (st != FQ_OK) { fq_task_destroy(t1); fq_queue_destroy(q); return 5; }
    st = fq_task_create(&t3, (fq_task_fn)3, (void *)3, NULL);
    if (st != FQ_OK) { fq_task_destroy(t1); fq_task_destroy(t2); fq_queue_destroy(q); return 6; }

    /* Push 3 items. */
    st = fq_queue_push(q, t1);
    if (st != FQ_OK) { fq_task_destroy(t1); fq_task_destroy(t2); fq_task_destroy(t3); fq_queue_destroy(q); return 7; }
    st = fq_queue_push(q, t2);
    if (st != FQ_OK) { fq_task_destroy(t2); fq_task_destroy(t3); fq_queue_destroy(q); return 8; }
    st = fq_queue_push(q, t3);
    if (st != FQ_OK) { fq_task_destroy(t3); fq_queue_destroy(q); return 9; }

    if (fq_queue_size(q) != 3) { fq_queue_destroy(q); return 10; }

    /* Pop in FIFO order. */
    fq_task_t *out = NULL;
    st = fq_queue_pop(q, &out);
    if (st != FQ_OK || out != t1) { fq_queue_destroy(q); return 11; }
    st = fq_queue_pop(q, &out);
    if (st != FQ_OK || out != t2) { fq_queue_destroy(q); return 12; }
    st = fq_queue_pop(q, &out);
    if (st != FQ_OK || out != t3) { fq_queue_destroy(q); return 13; }

    if (!fq_queue_empty(q)) { fq_queue_destroy(q); return 14; }

    /* Pop from empty. */
    st = fq_queue_pop(q, &out);
    if (st != FQ_ERR_BUSY) { fq_queue_destroy(q); return 15; }

    fq_task_destroy(t1);
    fq_task_destroy(t2);
    fq_task_destroy(t3);
    fq_queue_destroy(q);
    return 0;
}

 

typedef struct {
    fq_queue_t *queue;
    int         count;
    int         id;
} concurrent_arg_t;

static int producer_func(void *arg)
{
    concurrent_arg_t *ca = (concurrent_arg_t *)arg;
    for (int i = 0; i < ca->count; ++i) {
        fq_task_t *t = NULL;
        fq_task_create(&t, (fq_task_fn)(uintptr_t)(i + 1), NULL, NULL);
        fq_queue_push(ca->queue, t);
    }
    return 0;
}

static int consumer_func(void *arg)
{
    concurrent_arg_t *ca = (concurrent_arg_t *)arg;
    int consumed = 0;
    for (;;) {
        fq_task_t *t = NULL;
        if (fq_queue_pop(ca->queue, &t) == FQ_OK) {
            fq_task_destroy(t);
            consumed++;
        } else {
            fq_thread_yield();
        }
        if (consumed >= ca->count) break;
    }
    return 0;
}

int test_queue_concurrent(void)
{
    fq_queue_t *q = NULL;
    if (fq_queue_create(&q, 2048, NULL) != FQ_OK) return 1;

    const int NUM = 1000;

    concurrent_arg_t prod = { q, NUM, 0 };
    concurrent_arg_t cons = { q, NUM, 1 };

    fq_thread_t producer, consumer;
    if (fq_thread_create(&producer, producer_func, &prod) != 0) return 2;
    if (fq_thread_create(&consumer, consumer_func, &cons) != 0) return 3;

    fq_thread_join(&producer, NULL);
    fq_thread_join(&consumer, NULL);

    if (!fq_queue_empty(q)) return 4;

    fq_queue_destroy(q);
    return 0;
}
