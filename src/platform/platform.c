/**
 * @file platform.c
 * @brief Platform thread / mutex / condition-variable implementation.
 */

#if defined(__APPLE__)
#    if !defined(_DARWIN_C_SOURCE)
#        define _DARWIN_C_SOURCE
#    endif
#elif defined(__linux__) || defined(__unix__)
#    if !defined(_GNU_SOURCE)
#        define _GNU_SOURCE
#    endif
#    if !defined(_POSIX_C_SOURCE)
#        define _POSIX_C_SOURCE 200809L
#    endif
#endif

#include "fastqueue/platform.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════════
 *  POSIX implementation
 * ═══════════════════════════════════════════════════════════════════════ */

#ifdef FQ_OS_POSIX

 

struct fq_thread_arg_t {
    fq_thread_func_t func;
    void            *arg;
};

static void *posix_thread_entry(void *raw)
{
    struct fq_thread_arg_t ta;
    memcpy(&ta, raw, sizeof(ta));
    free(raw);
    int rc = ta.func(ta.arg);
    return (void *)(intptr_t)rc;
}

int fq_thread_create(fq_thread_t *thread, fq_thread_func_t func, void *arg)
{
    if (!thread || !func) return -1;

    struct fq_thread_arg_t *ta = malloc(sizeof(*ta));
    if (!ta) return -1;
    ta->func = func;
    ta->arg  = arg;

    int rc = pthread_create(&thread->handle, NULL, posix_thread_entry, ta);
    if (rc != 0) {
        free(ta);
        return -1;
    }
    return 0;
}

int fq_thread_join(fq_thread_t *thread, int *exit_code)
{
    if (!thread) return -1;
    void *retval = NULL;
    int rc = pthread_join(thread->handle, &retval);
    if (exit_code) *exit_code = (int)(intptr_t)retval;
    return rc == 0 ? 0 : -1;
}

int fq_thread_detach(fq_thread_t *thread)
{
    if (!thread) return -1;
    return pthread_detach(thread->handle) == 0 ? 0 : -1;
}

unsigned fq_hardware_concurrency(void)
{
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    return n > 0 ? (unsigned)n : 1;
}

void fq_thread_yield(void)
{
    sched_yield();
}

void fq_thread_sleep_ms(unsigned ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

 

int fq_mutex_init(fq_mutex_t *mutex)
{
    if (!mutex) return -1;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int rc = pthread_mutex_init(&mutex->handle, &attr);
    pthread_mutexattr_destroy(&attr);
    return rc == 0 ? 0 : -1;
}

void fq_mutex_destroy(fq_mutex_t *mutex)
{
    if (mutex) pthread_mutex_destroy(&mutex->handle);
}

void fq_mutex_lock(fq_mutex_t *mutex)
{
    if (mutex) pthread_mutex_lock(&mutex->handle);
}

int fq_mutex_trylock(fq_mutex_t *mutex)
{
    if (!mutex) return -1;
    return pthread_mutex_trylock(&mutex->handle) == 0 ? 0 : -1;
}

void fq_mutex_unlock(fq_mutex_t *mutex)
{
    if (mutex) pthread_mutex_unlock(&mutex->handle);
}

 

int fq_condition_init(fq_condition_t *cond)
{
    if (!cond) return -1;
    return pthread_cond_init(&cond->handle, NULL) == 0 ? 0 : -1;
}

void fq_condition_destroy(fq_condition_t *cond)
{
    if (cond) pthread_cond_destroy(&cond->handle);
}

void fq_condition_wait(fq_condition_t *cond, fq_mutex_t *mutex)
{
    if (cond && mutex) pthread_cond_wait(&cond->handle, &mutex->handle);
}

int fq_condition_timedwait(fq_condition_t *cond, fq_mutex_t *mutex,
                           unsigned timeout_ms)
{
    if (!cond || !mutex) return -1;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec  += timeout_ms / 1000;
    ts.tv_nsec += (long)(timeout_ms % 1000) * 1000000L;
    if (ts.tv_nsec >= 1000000000L) {
        ts.tv_sec  += 1;
        ts.tv_nsec -= 1000000000L;
    }
    return pthread_cond_timedwait(&cond->handle, &mutex->handle, &ts) == 0
               ? 0
               : -1;
}

void fq_condition_signal(fq_condition_t *cond)
{
    if (cond) pthread_cond_signal(&cond->handle);
}

void fq_condition_broadcast(fq_condition_t *cond)
{
    if (cond) pthread_cond_broadcast(&cond->handle);
}

#endif /* FQ_OS_POSIX */

/* ═══════════════════════════════════════════════════════════════════════
 *  Windows implementation
 * ═══════════════════════════════════════════════════════════════════════ */

#ifdef FQ_OS_WINDOWS

 

struct fq_thread_arg_t {
    fq_thread_func_t func;
    void            *arg;
};

static unsigned __stdcall win32_thread_entry(void *raw)
{
    struct fq_thread_arg_t ta;
    memcpy(&ta, raw, sizeof(ta));
    free(raw);
    int rc = ta.func(ta.arg);
    return (unsigned)rc;
}

int fq_thread_create(fq_thread_t *thread, fq_thread_func_t func, void *arg)
{
    if (!thread || !func) return -1;

    struct fq_thread_arg_t *ta = malloc(sizeof(*ta));
    if (!ta) return -1;
    ta->func = func;
    ta->arg  = arg;

    thread->handle = (HANDLE)_beginthreadex(
        NULL, 0, win32_thread_entry, ta, 0, &thread->id);

    if (thread->handle == 0) {
        free(ta);
        return -1;
    }
    return 0;
}

int fq_thread_join(fq_thread_t *thread, int *exit_code)
{
    if (!thread || !thread->handle) return -1;
    DWORD rc = WaitForSingleObject(thread->handle, INFINITE);
    if (rc == WAIT_OBJECT_0) {
        DWORD code = 0;
        GetExitCodeThread(thread->handle, &code);
        if (exit_code) *exit_code = (int)code;
        CloseHandle(thread->handle);
        thread->handle = NULL;
        return 0;
    }
    return -1;
}

int fq_thread_detach(fq_thread_t *thread)
{
    if (!thread || !thread->handle) return -1;
    CloseHandle(thread->handle);
    thread->handle = NULL;
    return 0;
}

unsigned fq_hardware_concurrency(void)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (unsigned)si.dwNumberOfProcessors;
}

void fq_thread_yield(void)
{
    YieldProcessor();
}

void fq_thread_sleep_ms(unsigned ms)
{
    Sleep((DWORD)ms);
}

 

int fq_mutex_init(fq_mutex_t *mutex)
{
    if (!mutex) return -1;
    InitializeSRWLock(&mutex->handle);
    return 0;
}

void fq_mutex_destroy(fq_mutex_t *mutex)
{
    (void)mutex;
}

void fq_mutex_lock(fq_mutex_t *mutex)
{
    if (mutex) AcquireSRWLockExclusive(&mutex->handle);
}

int fq_mutex_trylock(fq_mutex_t *mutex)
{
    if (!mutex) return -1;
    return TryAcquireSRWLockExclusive(&mutex->handle) ? 0 : -1;
}

void fq_mutex_unlock(fq_mutex_t *mutex)
{
    if (mutex) ReleaseSRWLockExclusive(&mutex->handle);
}

 

int fq_condition_init(fq_condition_t *cond)
{
    if (!cond) return -1;
    InitializeConditionVariable(&cond->handle);
    return 0;
}

void fq_condition_destroy(fq_condition_t *cond)
{
    (void)cond;
}

void fq_condition_wait(fq_condition_t *cond, fq_mutex_t *mutex)
{
    if (cond && mutex)
        SleepConditionVariableSRW(&cond->handle, &mutex->handle, INFINITE,
                                  0);
}

int fq_condition_timedwait(fq_condition_t *cond, fq_mutex_t *mutex,
                           unsigned timeout_ms)
{
    if (!cond || !mutex) return -1;
    return SleepConditionVariableSRW(&cond->handle, &mutex->handle,
                                     (DWORD)timeout_ms, 0)
               ? 0
               : -1;
}

void fq_condition_signal(fq_condition_t *cond)
{
    if (cond) WakeConditionVariable(&cond->handle);
}

void fq_condition_broadcast(fq_condition_t *cond)
{
    if (cond) WakeAllConditionVariable(&cond->handle);
}

#endif /* FQ_OS_WINDOWS */
