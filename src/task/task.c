/**
 * @file task.c
 * @brief Task creation and management.
 */

#include "fastqueue/task.h"
#include "fastqueue/future.h"
#include "fastqueue/memory.h"
#include "fastqueue/config.h"
#include "fastqueue/atomic.h"

#include <string.h>

 

struct fq_task_t {
    fq_task_fn          callback;
    void               *user_data;
    fq_completion_fn    completion;
    void               *completion_data;
    fq_future_t        *future;
    fq_priority_t       priority;
    fq_atomic_int_t     executed;
    fq_atomic_int_t     canceled;
    const fq_allocator_t *allocator;
};

 

fq_status_t fq_task_create(fq_task_t **task, fq_task_fn callback,
                           void *user_data, const fq_allocator_t *allocator)
{
    return fq_task_create_with_completion(task, callback, user_data,
                                          NULL, NULL, allocator);
}

fq_status_t fq_task_create_with_completion(fq_task_t **task,
                                           fq_task_fn callback,
                                           void *user_data,
                                           fq_completion_fn completion,
                                           void *completion_data,
                                           const fq_allocator_t *allocator)
{
    if (!task || !callback) return FQ_ERR_INVAL;
    if (!allocator) allocator = fq_default_allocator();

    fq_task_t *t = fq_calloc(allocator, 1, sizeof(*t));
    if (!t) return FQ_ERR_NOMEM;

    t->callback         = callback;
    t->user_data        = user_data;
    t->completion       = completion;
    t->completion_data  = completion_data;
    t->future           = NULL;
    t->priority         = FQ_PRIORITY_NORMAL;
    t->allocator        = allocator;

    fq_atomic_store_explicit(&t->executed, 0, FQ_MEMORY_ORDER_RELAXED);
    fq_atomic_store_explicit(&t->canceled, 0, FQ_MEMORY_ORDER_RELAXED);

    *task = t;
    return FQ_OK;
}

void fq_task_destroy(fq_task_t *task)
{
    if (!task) return;

    /* If this task has a pending future, cancel it. */
    if (task->future) {
        fq_future_cancel(task->future);
    }

    fq_free(task->allocator, task);
}

 

void *fq_task_user_data(const fq_task_t *task)
{
    return task ? task->user_data : NULL;
}

fq_priority_t fq_task_priority(const fq_task_t *task)
{
    return task ? task->priority : FQ_PRIORITY_NORMAL;
}

void fq_task_set_priority(fq_task_t *task, fq_priority_t priority)
{
    if (task) task->priority = priority;
}

void fq_task_set_future(fq_task_t *task, fq_future_t *future)
{
    if (task) task->future = future;
}

fq_future_t *fq_task_future(const fq_task_t *task)
{
    return task ? task->future : NULL;
}

fq_bool_t fq_task_is_executed(const fq_task_t *task)
{
    if (!task) return FQ_FALSE;
    return fq_atomic_load_explicit(
               &((fq_task_t *)task)->executed, FQ_MEMORY_ORDER_ACQUIRE)
                   ? FQ_TRUE
                   : FQ_FALSE;
}

fq_bool_t fq_task_is_canceled(const fq_task_t *task)
{
    if (!task) return FQ_FALSE;
    return fq_atomic_load_explicit(
               &((fq_task_t *)task)->canceled, FQ_MEMORY_ORDER_ACQUIRE)
                   ? FQ_TRUE
                   : FQ_FALSE;
}

 

/**
 * @brief Execute a task's callback and invoke completion.
 *
 * This is an internal function exposed for the scheduler.  Do not call
 * directly from user code.
 *
 * @param task Task to execute.
 */
void fq_task_execute(fq_task_t *task)
{
    if (!task) return;

    fq_bool_t was_canceled = fq_task_is_canceled(task);

    if (!was_canceled) {
        task->callback(task->user_data);
    }

    fq_atomic_store_explicit(&task->executed, 1, FQ_MEMORY_ORDER_RELEASE);

    /* Fulfill the future if one is attached. */
    if (task->future) {
        fq_future_set_result(task->future,
                             was_canceled ? FQ_ERR_CANCELED : FQ_OK);
    }

    /* Invoke completion callback. */
    if (task->completion) {
        task->completion(task->completion_data,
                         was_canceled ? FQ_ERR_CANCELED : FQ_OK);
    }
}
