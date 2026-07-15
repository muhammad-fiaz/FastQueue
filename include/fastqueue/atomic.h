#ifndef FQ_ATOMIC_H
#define FQ_ATOMIC_H

#include "config.h"
#include "platform.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(FQ_COMPILER_MSVC) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#define FQ_HAS_STDATOMIC 1
#else
#define FQ_HAS_STDATOMIC 0
#endif

#define FQ_MEMORY_ORDER_RELAXED 0
#define FQ_MEMORY_ORDER_ACQUIRE 1
#define FQ_MEMORY_ORDER_RELEASE 2
#define FQ_MEMORY_ORDER_ACQ_REL 3
#define FQ_MEMORY_ORDER_SEQ_CST 4

#if FQ_HAS_STDATOMIC

typedef _Atomic int           fq_atomic_int_t;
typedef _Atomic unsigned      fq_atomic_uint_t;
typedef _Atomic long          fq_atomic_long_t;
typedef _Atomic unsigned long fq_atomic_ulong_t;

#define FQ_ATOMIC_INIT(val) ATOMIC_VAR_INIT(val)

static inline int fq_atomic_load_explicit(volatile fq_atomic_int_t *a, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_load_explicit((_Atomic int *)(void *)a, (memory_order)mo);
}

static inline void fq_atomic_store_explicit(volatile fq_atomic_int_t *a, int val, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    atomic_store_explicit((_Atomic int *)(void *)a, val, (memory_order)mo);
}

static inline int fq_atomic_exchange_explicit(volatile fq_atomic_int_t *a, int val, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_exchange_explicit((_Atomic int *)(void *)a, val, (memory_order)mo);
}

static inline fq_bool_t fq_atomic_compare_exchange_strong_explicit(
    volatile fq_atomic_int_t *a, int *expected, int desired,
    int success_order, int failure_order)
{
    int suc, fail;
    switch (success_order) {
    case 1: suc = memory_order_acquire; break;
    case 2: suc = memory_order_release; break;
    case 3: suc = memory_order_acq_rel; break;
    default: suc = (success_order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    switch (failure_order) {
    case 1: fail = memory_order_acquire; break;
    default: fail = (failure_order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_compare_exchange_strong_explicit(
        (_Atomic int *)(void *)a, expected, desired,
        (memory_order)suc, (memory_order)fail) ? 1 : 0;
}

static inline fq_bool_t fq_atomic_compare_exchange_weak_explicit(
    volatile fq_atomic_int_t *a, int *expected, int desired,
    int success_order, int failure_order)
{
    int suc, fail;
    switch (success_order) {
    case 1: suc = memory_order_acquire; break;
    case 2: suc = memory_order_release; break;
    case 3: suc = memory_order_acq_rel; break;
    default: suc = (success_order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    switch (failure_order) {
    case 1: fail = memory_order_acquire; break;
    default: fail = (failure_order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_compare_exchange_weak_explicit(
        (_Atomic int *)(void *)a, expected, desired,
        (memory_order)suc, (memory_order)fail) ? 1 : 0;
}

static inline int fq_atomic_fetch_add_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_fetch_add_explicit((_Atomic int *)(void *)a, operand, (memory_order)mo);
}

static inline int fq_atomic_fetch_sub_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_fetch_sub_explicit((_Atomic int *)(void *)a, operand, (memory_order)mo);
}

static inline int fq_atomic_fetch_or_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_fetch_or_explicit((_Atomic int *)(void *)a, operand, (memory_order)mo);
}

static inline int fq_atomic_fetch_and_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    int mo;
    switch (order) {
    case 1: mo = memory_order_acquire; break;
    case 2: mo = memory_order_release; break;
    case 3: mo = memory_order_acq_rel; break;
    default: mo = (order <= 0) ? memory_order_relaxed : memory_order_seq_cst; break;
    }
    return atomic_fetch_and_explicit((_Atomic int *)(void *)a, operand, (memory_order)mo);
}

#else

typedef int           fq_atomic_int_t;
typedef unsigned      fq_atomic_uint_t;
typedef long          fq_atomic_long_t;
typedef unsigned long fq_atomic_ulong_t;

#define FQ_ATOMIC_INIT(val) (val)

#if defined(FQ_COMPILER_MSVC)

#include <intrin.h>
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedOr)
#pragma intrinsic(_InterlockedAnd)

static inline int fq_atomic_load_explicit(fq_atomic_int_t *a, int order)
{
    (void)order;
    return (int)_InterlockedCompareExchange((volatile long *)a, 0, 0);
}

static inline void fq_atomic_store_explicit(fq_atomic_int_t *a, int val, int order)
{
    (void)order;
    _InterlockedExchange((volatile long *)a, (long)val);
}

static inline int fq_atomic_exchange_explicit(fq_atomic_int_t *a, int val, int order)
{
    (void)order;
    return (int)_InterlockedExchange((volatile long *)a, (long)val);
}

static inline fq_bool_t fq_atomic_compare_exchange_strong_explicit(
    fq_atomic_int_t *a, int *expected, int desired,
    int success_order, int failure_order)
{
    (void)success_order;
    (void)failure_order;
    long old = (long)*expected;
    long prev = _InterlockedCompareExchange((volatile long *)a, (long)desired, old);
    if (prev == old) return 1;
    *expected = (int)prev;
    return 0;
}

static inline fq_bool_t fq_atomic_compare_exchange_weak_explicit(
    fq_atomic_int_t *a, int *expected, int desired,
    int success_order, int failure_order)
{
    return fq_atomic_compare_exchange_strong_explicit(a, expected, desired, success_order, failure_order);
}

static inline int fq_atomic_fetch_add_explicit(fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return (int)_InterlockedExchangeAdd((volatile long *)a, (long)operand);
}

static inline int fq_atomic_fetch_sub_explicit(fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return (int)_InterlockedExchangeAdd((volatile long *)a, (long)(-operand));
}

static inline int fq_atomic_fetch_or_explicit(fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return (int)_InterlockedOr((volatile long *)a, (long)operand);
}

static inline int fq_atomic_fetch_and_explicit(fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return (int)_InterlockedAnd((volatile long *)a, (long)operand);
}

#elif defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)

static inline int fq_atomic_load_explicit(volatile fq_atomic_int_t *a, int order)
{
    int val = *a;
    if (order >= 1) __atomic_thread_fence(__ATOMIC_ACQUIRE);
    return val;
}

static inline void fq_atomic_store_explicit(volatile fq_atomic_int_t *a, int val, int order)
{
    if (order >= 2) __atomic_thread_fence(__ATOMIC_RELEASE);
    *a = val;
}

static inline int fq_atomic_exchange_explicit(volatile fq_atomic_int_t *a, int val, int order)
{
    (void)order;
    return __sync_lock_test_and_set(a, val);
}

static inline fq_bool_t fq_atomic_compare_exchange_strong_explicit(
    volatile fq_atomic_int_t *a, int *expected, int desired,
    int success_order, int failure_order)
{
    (void)success_order;
    (void)failure_order;
    return __sync_bool_compare_and_swap(a, *expected, desired) ? 1 : (*expected = *a, 0);
}

static inline fq_bool_t fq_atomic_compare_exchange_weak_explicit(
    volatile fq_atomic_int_t *a, int *expected, int desired,
    int success_order, int failure_order)
{
    return fq_atomic_compare_exchange_strong_explicit(a, expected, desired, success_order, failure_order);
}

static inline int fq_atomic_fetch_add_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return __sync_fetch_and_add(a, operand);
}

static inline int fq_atomic_fetch_sub_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return __sync_fetch_and_sub(a, operand);
}

static inline int fq_atomic_fetch_or_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return __sync_fetch_and_or(a, operand);
}

static inline int fq_atomic_fetch_and_explicit(volatile fq_atomic_int_t *a, int operand, int order)
{
    (void)order;
    return __sync_fetch_and_and(a, operand);
}

#endif
#endif

#define fq_atomic_load(a) fq_atomic_load_explicit(a, 4)
#define fq_atomic_store(a, val) fq_atomic_store_explicit(a, val, 4)
#define fq_atomic_exchange(a, val) fq_atomic_exchange_explicit(a, val, 4)
#define fq_atomic_cas_strong(a, expected, desired) fq_atomic_compare_exchange_strong_explicit(a, expected, desired, 4, 4)
#define fq_atomic_cas_weak(a, expected, desired) fq_atomic_compare_exchange_weak_explicit(a, expected, desired, 4, 4)
#define fq_atomic_fetch_add(a, val) fq_atomic_fetch_add_explicit(a, val, 4)
#define fq_atomic_fetch_sub(a, val) fq_atomic_fetch_sub_explicit(a, val, 4)

#ifdef __cplusplus
}
#endif

#endif /* FQ_ATOMIC_H */