---
title: FAQ
description: Frequently asked questions about FastQueue C/C++ thread pool and job system library.
keywords: faq, frequently asked questions, help, support, c, c++
---

# Frequently Asked Questions

## General

### What is FastQueue?

FastQueue is a production-ready, high-performance C/C++ job system and thread pool library. It provides work-stealing thread pools, MPMC queues, futures, and custom allocator support.

### What language is it written in?

FastQueue is written in C (C99 and later) with full C++ compatibility. All headers include `extern "C"` guards for seamless use in C++ projects.

### What platforms are supported?

- Linux (GCC, Clang)
- Windows (MSVC 2022+)
- macOS (Clang)

### Is it header-only?

No. FastQueue is a compiled library. You link against `fastqueue.lib` (or `libfastqueue.a` on Linux/macOS).

## Usage

### How many threads should I use?

For CPU-bound tasks, use the number of physical CPU cores:
```c
cfg.thread_count = 0; // Auto-detect hardware concurrency
```

For I/O-bound tasks, you can use more threads:
```c
cfg.thread_count = 16;
```

### How do I cancel all tasks?

```c
fq_scheduler_cancel_all(scheduler);
```

### Can I use it from C++?

Yes. All headers have `extern "C"` guards:
```cpp
#include <fastqueue/fastqueue.h>
// Works directly in C++
```

### How do I handle errors?

All functions return `fq_status_t`:
```c
fq_status_t st = fq_thread_pool_submit_fn(pool, my_task, NULL);
if (st != FQ_OK) {
    fprintf(stderr, "Error: %s\n", fq_status_string(st));
}
```

## Building

### CMake minimum version?

CMake 3.20 or newer.

### Compiler requirements?

- GCC 4.8+ (C99), GCC 11+ (recommended for full C11/C17 support)
- Clang 3.0+ (C99), Clang 10+ (recommended)
- MSVC 2015+ (C99), MSVC 2019+ (recommended)

### Can I build as shared library?

Yes:
```bash
cmake -B build -DFQ_SHARED=ON
```

### How do I enable sanitizers?

```bash
cmake -B build -DFQ_ENABLE_ASAN=ON -DFQ_ENABLE_UBSAN=ON
```

## Performance

### Is it lock-free?

The queue uses mutex protection for MPMC safety. The scheduler uses lock-free atomic operations for task counting.

### What's the maximum throughput?

Benchmarked at ~2M jobs/sec on a 4-core system with trivial tasks.

### How does work stealing work?

When a worker's local queue is empty, it tries to steal tasks from other workers' queues. This ensures all cores stay busy.
