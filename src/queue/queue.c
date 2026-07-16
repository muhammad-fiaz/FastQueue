/**
 * @file queue.c
 * @brief Thread-safe FIFO task queue implementation.
 *
 * Uses a circular buffer with atomic head/tail indices.
 * Power-of-two capacity enables fast modular arithmetic via masking.
 */

#include "fastqueue/queue.h"
#include "fastqueue/task.h"
#include "fastqueue/atomic.h"
#include "fastqueue/memory.h"
#include "fastqueue/config.h"
#include "fastqueue/platform.h"

#include <string.h>

 

struct fq_queue_t {
    fq_task_t           **buffer;
    size_t                capacity;
    size_t                mask;
    fq_atomic_int_t       head;  /* next read position */
    fq_atomic_int_t       tail;  /* next write position */
    fq_atomic_int_t       closed;
    fq_mutex_t            push_lock;
    fq_mutex_t            pop_lock;
    const fq_allocator_t *allocator;
};

static size_t next_pow2(size_t v)
{
    if (v == 0) return 1;
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
#if defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8)
    v |= v >> 32;
#endif
    return v + 1;
}

 

fq_status_t fq_queue_create(fq_queue_t **queue, size_t capacity,
                            const fq_allocator_t *allocator)
{
    if (!queue) return FQ_ERR_INVAL;
    if (!allocator) allocator = fq_default_allocator();

    capacity = next_pow2(capacity < FQ_QUEUE_MIN_CAPACITY
                             ? FQ_QUEUE_MIN_CAPACITY
                             : capacity);
    if (capacity > FQ_QUEUE_MAX_CAPACITY) return FQ_ERR_INVAL;

    fq_queue_t *q = fq_calloc(allocator, 1, sizeof(*q));
    if (!q) return FQ_ERR_NOMEM;

    q->buffer = fq_calloc(allocator, capacity, sizeof(fq_task_t *));
    if (!q->buffer) {
        fq_free(allocator, q);
        return FQ_ERR_NOMEM;
    }

    q->capacity  = capacity;
    q->mask      = capacity - 1;
    q->allocator = allocator;

    fq_atomic_store_explicit(&q->head, 0, FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_store_explicit(&q->tail, 0, FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_store_explicit(&q->closed, 0, FQ_MEMORY_ORDER_RELAXED);
    fq_mutex_init(&q->push_lock);
    fq_mutex_init(&q->pop_lock);

    *queue = q;
    return FQ_OK;
}

void fq_queue_destroy(fq_queue_t *queue)
{
    if (!queue) return;
    fq_mutex_destroy(&queue->push_lock);
    fq_mutex_destroy(&queue->pop_lock);
    fq_free(queue->allocator, queue->buffer);
    fq_free(queue->allocator, queue);
}

 

fq_status_t fq_queue_push(fq_queue_t *queue, fq_task_t *task)
{
    if (!queue || !task) return FQ_ERR_INVAL;
    if (fq_atomic_load_explicit(&queue->closed, FQ_MEMORY_ORDER_ACQUIRE))
        return FQ_ERR_CLOSED;

    fq_mutex_lock(&queue->push_lock);

    int tail = fq_atomic_load_explicit(&queue->tail, FQ_MEMORY_ORDER_RELAXED);
    int next = (tail + 1) & (int)queue->mask;

    int head = fq_atomic_load_explicit(&queue->head, FQ_MEMORY_ORDER_ACQUIRE);
    if (next == head) {
        fq_mutex_unlock(&queue->push_lock);
        return FQ_ERR_OVERFLOW;
    }

    queue->buffer[(size_t)tail] = task;

    fq_atomic_store_explicit(&queue->tail, next, FQ_MEMORY_ORDER_RELEASE);
    fq_mutex_unlock(&queue->push_lock);
    return FQ_OK;
}

fq_status_t fq_queue_pop(fq_queue_t *queue, fq_task_t **task)
{
    if (!queue || !task) return FQ_ERR_INVAL;

    if (fq_mutex_trylock(&queue->pop_lock) != 0) {
        fq_mutex_lock(&queue->pop_lock);
    }

    int head = fq_atomic_load_explicit(&queue->head, FQ_MEMORY_ORDER_RELAXED);
    int tail = fq_atomic_load_explicit(&queue->tail, FQ_MEMORY_ORDER_ACQUIRE);
    if (head == tail) {
        fq_mutex_unlock(&queue->pop_lock);
        return FQ_ERR_BUSY;
    }

    *task = queue->buffer[(size_t)head];

    fq_atomic_store_explicit(&queue->head,
                             (head + 1) & (int)queue->mask,
                             FQ_MEMORY_ORDER_RELEASE);
    fq_mutex_unlock(&queue->pop_lock);
    return FQ_OK;
}

fq_status_t fq_queue_try_pop(fq_queue_t *queue, fq_task_t **task)
{
    return fq_queue_pop(queue, task);
}

 

size_t fq_queue_size(const fq_queue_t *queue)
{
    if (!queue) return 0;
    int head = fq_atomic_load_explicit(
        &((fq_queue_t *)queue)->head, FQ_MEMORY_ORDER_RELAXED);
    int tail = fq_atomic_load_explicit(
        &((fq_queue_t *)queue)->tail, FQ_MEMORY_ORDER_RELAXED);
    return (size_t)((tail - head) & (int)queue->mask);
}

fq_bool_t fq_queue_empty(const fq_queue_t *queue)
{
    return fq_queue_size(queue) == 0 ? FQ_TRUE : FQ_FALSE;
}

size_t fq_queue_capacity(const fq_queue_t *queue)
{
    return queue ? queue->capacity - 1 : 0;
}

size_t fq_queue_drain(fq_queue_t *queue, fq_task_t **out, size_t max)
{
    if (!queue || !out || max == 0) return 0;

    size_t count = 0;
    while (count < max) {
        fq_task_t *t = NULL;
        if (fq_queue_pop(queue, &t) != FQ_OK) break;
        out[count++] = t;
    }
    return count;
}
