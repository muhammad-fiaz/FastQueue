/**
 * @file platform.h
 * @brief Platform detection and abstraction for FastQueue.
 *
 * This header detects the target operating system, compiler, and CPU
 * architecture at compile time, and exposes platform-specific primitives
 * (threads, mutexes, condition variables) behind a uniform interface.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FQ_PLATFORM_H
#define FQ_PLATFORM_H

 

/**
 * @def FQ_C_VERSION
 * @brief Encoded C standard version as a single integer (major * 100 + minor).
 *
 * Defaults to 2300 (C23) when the compiler does not define `__STDC_VERSION__`.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#    define FQ_C_VERSION 2300
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201710L
#    define FQ_C_VERSION 1700
#else
#    define FQ_C_VERSION 2300 /* assume C23 capable compiler */
#endif

/**
 * @def FQ_C23_FEATURES
 * @brief Defined when C23 features are available.
 */
#if FQ_C_VERSION >= 2300
#    define FQ_C23_FEATURES 1
#endif

 

/**
 * @def FQ_COMPILER_GCC
 * @brief Defined when compiling with GCC.
 */
#if defined(__GNUC__) && !defined(__clang__)
#    define FQ_COMPILER_GCC 1
#endif

/**
 * @def FQ_COMPILER_CLANG
 * @brief Defined when compiling with Clang.
 */
#if defined(__clang__)
#    define FQ_COMPILER_CLANG 1
#endif

/**
 * @def FQ_COMPILER_MSVC
 * @brief Defined when compiling with MSVC.
 */
#if defined(_MSC_VER)
#    define FQ_COMPILER_MSVC 1
#endif

 

/**
 * @def FQ_OS_LINUX
 * @brief Defined on Linux.
 */
#if defined(__linux__)
#    define FQ_OS_LINUX 1
#endif

/**
 * @def FQ_OS_WINDOWS
 * @brief Defined on Windows.
 */
#if defined(_WIN32) || defined(_WIN64)
#    define FQ_OS_WINDOWS 1
#endif

/**
 * @def FQ_OS_MACOS
 * @brief Defined on macOS / Darwin.
 */
#if defined(__APPLE__) && defined(__MACH__)
#    define FQ_OS_MACOS 1
#endif

/**
 * @def FQ_OS_POSIX
 * @brief Defined on any POSIX-conformant system (Linux, macOS).
 */
#if defined(FQ_OS_LINUX) || defined(FQ_OS_MACOS)
#    define FQ_OS_POSIX 1
#endif

 

/**
 * @def FQ_ARCH_X86_64
 * @brief Defined on x86-64 / AMD64.
 */
#if defined(__x86_64__) || defined(_M_X64)
#    define FQ_ARCH_X86_64 1
#endif

/**
 * @def FQ_ARCH_ARM64
 * @brief Defined on AArch64 / ARM64.
 */
#if defined(__aarch64__) || defined(_M_ARM64)
#    define FQ_ARCH_ARM64 1
#endif

/**
 * @def FQ_ARCH_X86
 * @brief Defined on 32-bit x86.
 */
#if defined(__i386__) || defined(_M_IX86)
#    define FQ_ARCH_X86 1
#endif

/**
 * @def FQ_ARCH_ARM
 * @brief Defined on 32-bit ARM.
 */
#if defined(__arm__) || defined(_M_ARM)
#    define FQ_ARCH_ARM 1
#endif

 

/**
 * @def FQ_CACHE_LINE_SIZE
 * @brief Assumed cache-line size in bytes for padding and alignment.
 *
 * 64 bytes covers the vast majority of modern x86 and ARM processors.
 */
#if !defined(FQ_CACHE_LINE_SIZE)
#    define FQ_CACHE_LINE_SIZE 64
#endif

 

/**
 * @def FQ_LIKELY
 * @brief Branch-prediction hint: condition is likely true.
 */
/**
 * @def FQ_UNLIKELY
 * @brief Branch-prediction hint: condition is likely false.
 */
#if defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_LIKELY(x)   __builtin_expect(!!(x), 1)
#    define FQ_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#    define FQ_LIKELY(x)   (x)
#    define FQ_UNLIKELY(x) (x)
#endif

/**
 * @def FQ_INLINE
 * @brief Request aggressive inlining.
 */
#if defined(FQ_COMPILER_MSVC)
#    define FQ_INLINE __forceinline
#elif defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_INLINE static inline __attribute__((always_inline))
#else
#    define FQ_INLINE static inline
#endif

/**
 * @def FQ_NOINLINE
 * @brief Prevent the compiler from inlining a function.
 */
#if defined(FQ_COMPILER_MSVC)
#    define FQ_NOINLINE __declspec(noinline)
#elif defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_NOINLINE __attribute__((noinline))
#else
#    define FQ_NOINLINE
#endif

/**
 * @def FQ_EXPORT
 * @brief Symbol visibility for shared-library builds.
 */
/**
 * @def FQ_IMPORT
 * @brief Symbol import for consumers of the shared library.
 */
/**
 * @def FQ_API
 * @brief Resolves to ::FQ_EXPORT or ::FQ_IMPORT depending on build context.
 */
#if defined(FQ_OS_WINDOWS)
#    if defined(FQ_BUILDING_SHARED)
#        define FQ_EXPORT __declspec(dllexport)
#    elif defined(FQ_SHARED)
#        define FQ_EXPORT __declspec(dllimport)
#    else
#        define FQ_EXPORT
#    endif
#else
#    if defined(FQ_BUILDING_SHARED)
#        define FQ_EXPORT __attribute__((visibility("default")))
#    else
#        define FQ_EXPORT
#    endif
#endif
#define FQ_IMPORT FQ_EXPORT

/**
 * @def FQ_API
 * @brief Public API export macro.
 */
#define FQ_API FQ_EXPORT

/**
 * @def FQ_NODISCARD
 * @brief Warn when a function return value is ignored.
 */
#if defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_NODISCARD __attribute__((warn_unused_result))
#elif defined(FQ_COMPILER_MSVC)
#    define FQ_NODISCARD _Check_return_
#else
#    define FQ_NODISCARD
#endif

/**
 * @def FQ_PRINTF_FMT
 * @brief Printf-format string checking attribute.
 */
#if defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_PRINTF_FMT(fmt, args) __attribute__((format(printf, fmt, args)))
#else
#    define FQ_PRINTF_FMT(fmt, args)
#endif

/**
 * @def FQ_DEPRECATED
 * @brief Mark a function or macro as deprecated.
 */
#if defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_DEPRECATED __attribute__((deprecated))
#elif defined(FQ_COMPILER_MSVC)
#    define FQ_DEPRECATED __declspec(deprecated)
#else
#    define FQ_DEPRECATED
#endif

/**
 * @def FQ_ALIGNAS
 * @brief Alignment specifier.
 *
 * @a n must be a power of two.
 */
#if defined(FQ_C23_FEATURES)
#    define FQ_ALIGNAS(n) alignas(n)
#elif defined(FQ_COMPILER_MSVC)
#    define FQ_ALIGNAS(n) __declspec(align(n))
#elif defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_ALIGNAS(n) __attribute__((aligned(n)))
#else
#    define FQ_ALIGNAS(n)
#endif

/**
 * @def FQ_STATIC_ASSERT
 * @brief Compile-time assertion.
 */
#if defined(FQ_C23_FEATURES)
#    define FQ_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif defined(FQ_COMPILER_MSVC)
#    define FQ_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
#    define FQ_STATIC_ASSERT(cond, msg) \
        typedef char fq_sa_##__LINE__[(cond) ? 1 : -1]
#endif

/**
 * @def FQ_UNUSED
 * @brief Silence unused-parameter warnings.
 */
#if defined(FQ_COMPILER_GCC) || defined(FQ_COMPILER_CLANG)
#    define FQ_UNUSED __attribute__((unused))
#elif defined(FQ_COMPILER_MSVC)
#    define FQ_UNUSED __pragma(warning(suppress : 4100))
#else
#    define FQ_UNUSED
#endif

 

#ifdef FQ_OS_POSIX
#    if defined(__APPLE__)
#        if !defined(_DARWIN_C_SOURCE)
#            define _DARWIN_C_SOURCE
#        endif
#    elif defined(__linux__) || defined(__unix__)
#        if !defined(_GNU_SOURCE)
#            define _GNU_SOURCE
#        endif
#    endif
#    include <pthread.h>
#    include <unistd.h>
#    include <time.h>
#endif

#ifdef FQ_OS_WINDOWS
#    define WIN32_LEAN_AND_MEAN
#    define NOMINMAX
#    include <windows.h>
#    include <process.h>
#endif

 

/**
 * @struct fq_thread_t
 * @brief Opaque handle to a platform thread.
 */
#ifdef FQ_OS_POSIX
typedef struct fq_thread_t {
    pthread_t handle;
} fq_thread_t;
#endif

#ifdef FQ_OS_WINDOWS
typedef struct fq_thread_t {
    HANDLE handle;
    unsigned id;
} fq_thread_t;
#endif

/**
 * @struct fq_mutex_t
 * @brief Mutual-exclusion lock.
 */
#ifdef FQ_OS_POSIX
typedef struct fq_mutex_t {
    pthread_mutex_t handle;
} fq_mutex_t;
#endif

#ifdef FQ_OS_WINDOWS
typedef struct fq_mutex_t {
    SRWLOCK handle;
} fq_mutex_t;
#endif

/**
 * @struct fq_condition_t
 * @brief Condition variable for thread synchronization.
 */
#ifdef FQ_OS_POSIX
typedef struct fq_condition_t {
    pthread_cond_t handle;
} fq_condition_t;
#endif

#ifdef FQ_OS_WINDOWS
typedef struct fq_condition_t {
    CONDITION_VARIABLE handle;
} fq_condition_t;
#endif

/**
 * @brief Thread start routine signature.
 *
 * @param arg User-defined context pointer.
 * @return Exit code (0 for success).
 */
typedef int (*fq_thread_func_t)(void *arg);

 

/**
 * @brief Create and start a thread.
 *
 * @param[out] thread  Pointer to thread handle.
 * @param      func    Thread entry point.
 * @param      arg     Argument forwarded to @p func.
 * @return 0 on success, -1 on failure.
 */
FQ_API int fq_thread_create(fq_thread_t *thread, fq_thread_func_t func, void *arg);

/**
 * @brief Wait for a thread to finish and retrieve its exit code.
 *
 * @param[in,out] thread    Thread handle.
 * @param[out]    exit_code Pointer to store the exit code (may be NULL).
 * @return 0 on success, -1 on error.
 */
FQ_API int fq_thread_join(fq_thread_t *thread, int *exit_code);

/**
 * @brief Detach a thread so its resources are freed on exit.
 *
 * @param[in,out] thread Thread handle.
 * @return 0 on success, -1 on error.
 */
FQ_API int fq_thread_detach(fq_thread_t *thread);

/**
 * @brief Return the number of hardware concurrency (cores).
 *
 * @return Positive core count, or 1 if undetectable.
 */
FQ_API unsigned fq_hardware_concurrency(void);

/**
 * @brief Yield the current thread's time slice.
 */
FQ_API void fq_thread_yield(void);

/**
 * @brief Sleep for at least @p ms milliseconds.
 *
 * @param ms Minimum sleep time in milliseconds.
 */
FQ_API void fq_thread_sleep_ms(unsigned ms);

 

/**
 * @brief Initialise a mutex.
 *
 * @param[out] mutex Mutex to initialise.
 * @return 0 on success, -1 on error.
 */
FQ_API int fq_mutex_init(fq_mutex_t *mutex);

/**
 * @brief Destroy a mutex.  Must not be held.
 *
 * @param[in,out] mutex Mutex to destroy.
 */
FQ_API void fq_mutex_destroy(fq_mutex_t *mutex);

/**
 * @brief Acquire a mutex (blocking).
 *
 * @param[in,out] mutex Mutex to lock.
 */
FQ_API void fq_mutex_lock(fq_mutex_t *mutex);

/**
 * @brief Try to acquire a mutex without blocking.
 *
 * @param[in,out] mutex Mutex to try.
 * @return 0 if acquired, -1 if not.
 */
FQ_API int fq_mutex_trylock(fq_mutex_t *mutex);

/**
 * @brief Release a mutex.
 *
 * @param[in,out] mutex Mutex to unlock.
 */
FQ_API void fq_mutex_unlock(fq_mutex_t *mutex);

 

/**
 * @brief Initialise a condition variable.
 *
 * @param[out] cond Condition variable to initialise.
 * @return 0 on success, -1 on error.
 */
FQ_API int fq_condition_init(fq_condition_t *cond);

/**
 * @brief Destroy a condition variable.
 *
 * @param[in,out] cond Condition variable to destroy.
 */
FQ_API void fq_condition_destroy(fq_condition_t *cond);

/**
 * @brief Block on a condition variable (mutex must be held).
 *
 * @param[in,out] cond  Condition variable.
 * @param[in,out] mutex Associated mutex (released atomically).
 */
FQ_API void fq_condition_wait(fq_condition_t *cond, fq_mutex_t *mutex);

/**
 * @brief Block on a condition variable with a timeout.
 *
 * @param[in,out] cond  Condition variable.
 * @param[in,out] mutex Associated mutex.
 * @param      timeout_ms  Timeout in milliseconds.
 * @return 0 on signal/ broadcast, -1 on timeout.
 */
FQ_API int fq_condition_timedwait(fq_condition_t *cond, fq_mutex_t *mutex,
                                  unsigned timeout_ms);

/**
 * @brief Wake one blocked thread.
 *
 * @param[in,out] cond Condition variable.
 */
FQ_API void fq_condition_signal(fq_condition_t *cond);

/**
 * @brief Wake all blocked threads.
 *
 * @param[in,out] cond Condition variable.
 */
FQ_API void fq_condition_broadcast(fq_condition_t *cond);

#endif /* FQ_PLATFORM_H */
