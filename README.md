<div align="center">

# FastQueue

<a href="https://muhammad-fiaz.github.io/FastQueue/"><img src="https://img.shields.io/badge/docs-muhammad--fiaz.github.io-blue" alt="Documentation"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue"><img src="https://img.shields.io/badge/C-C99%20%7C%20C11%20%7C%20C17%20%7C%20C23-blue.svg?logo=c" alt="C Standard"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue"><img src="https://img.shields.io/github/stars/muhammad-fiaz/FastQueue" alt="GitHub stars"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue/issues"><img src="https://img.shields.io/github/issues/muhammad-fiaz/FastQueue" alt="GitHub issues"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue/pulls"><img src="https://img.shields.io/github/issues-pr/muhammad-fiaz/FastQueue" alt="GitHub pull requests"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue"><img src="https://img.shields.io/github/last-commit/muhammad-fiaz/FastQueue" alt="GitHub last commit"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue"><img src="https://img.shields.io/github/license/muhammad-fiaz/FastQueue" alt="License"></a>
<a href="https://github.com/muhammad-fiaz/FastQueue/actions/workflows/ci.yml"><img src="https://github.com/muhammad-fiaz/FastQueue/actions/workflows/ci.yml/badge.svg" alt="CI"></a>
<img src="https://img.shields.io/badge/platforms-linux%20%7C%20windows%20%7C%20macos-blue" alt="Supported Platforms">
<a href="https://github.com/muhammad-fiaz/FastQueue/releases/latest"><img src="https://img.shields.io/github/v/release/muhammad-fiaz/FastQueue?label=Latest%20Release&style=flat-square" alt="Latest Release"></a>
<a href="https://pay.muhammadfiaz.com"><img src="https://img.shields.io/badge/Sponsor-pay.muhammadfiaz.com-ff69b4?style=flat&logo=heart" alt="Sponsor"></a>
<a href="https://github.com/sponsors/muhammad-fiaz"><img src="https://img.shields.io/badge/Sponsor-GitHub-pink?style=social&logo=github" alt="GitHub Sponsors"></a>
<a href="https://hits.sh/github.com/muhammad-fiaz/FastQueue/"><img src="https://hits.sh/github.com/muhammad-fiaz/FastQueue.svg?label=Visitors&extraCount=0&color=green" alt="Repo Visitors"></a>

<p><em>A high-performance, production-ready C/C++ job system and thread pool library (C99 and later).</em></p>

<b><a href="https://muhammad-fiaz.github.io/FastQueue/">Documentation</a> |
<a href="https://muhammad-fiaz.github.io/FastQueue/api/overview">API Reference</a> |
<a href="https://muhammad-fiaz.github.io/FastQueue/guide/getting-started">Quick Start</a> |
<a href="CONTRIBUTING.md">Contributing</a></b>

</div>

`FastQueue` is a modern, high-performance C/C++ library providing work-stealing thread pools, MPMC task queues, futures, and custom allocator support for building high-performance concurrent applications. Compatible with C99, C11, C17, C23, and C++.

> [!TIP]
> If you build with FastQueue, make sure to give it a star.

> [!NOTE]
> **Cross-platform:** FastQueue supports Linux (GCC, Clang), Windows (MSVC 2015+), and macOS (Clang) with zero external dependencies. C99 or later required, C++ fully supported.

---

<details>
<summary><strong>Features</strong> (click to expand)</summary>

| Feature | Description | Documentation |
|---------|-------------|---------------|
| **Thread Pool** | Built-in work-stealing thread pool with configurable worker count and automatic load balancing. | https://muhammad-fiaz.github.io/FastQueue/api/thread_pool |
| **Job Scheduler** | Low-level scheduler with priority support, work stealing, and statistics tracking. | https://muhammad-fiaz.github.io/FastQueue/api/scheduler |
| **MPMC Queue** | Thread-safe multi-producer multi-consumer queue with mutex-protected push/pop operations. | https://muhammad-fiaz.github.io/FastQueue/api/queue |
| **Futures** | Awaitable results with spin-then-wait strategy for low-latency task completion. | https://muhammad-fiaz.github.io/FastQueue/api/future |
| **Parallel For** | Built-in parallel for-loop that distributes work across worker threads. | https://muhammad-fiaz.github.io/FastQueue/api/parallel |
| **Time Utilities** | High-resolution timer and monotonic clock for performance measurement. | https://muhammad-fiaz.github.io/FastQueue/api/time |
| **Custom Allocators** | Pluggable allocator interface for memory-constrained and embedded environments. | https://muhammad-fiaz.github.io/FastQueue/guide/memory |
| **Cross-Platform** | Runs on Linux, Windows (MSVC), and macOS with no external dependencies. | https://muhammad-fiaz.github.io/FastQueue/guide/installation |
| **C99/C11/C17/C23** | Compatible with all modern C standards. C++ ready with `extern "C"` guards. | https://muhammad-fiaz.github.io/FastQueue/guide/installation |
| **C++ Compatible** | All headers have `extern "C"` guards for direct use in C++ projects. | https://muhammad-fiaz.github.io/FastQueue/api/overview |
| **Zero Dependencies** | Uses only the C standard library and platform APIs. | https://muhammad-fiaz.github.io/FastQueue/guide/installation |
| **Work Stealing** | Idle workers automatically steal tasks from busy workers for optimal utilization. | https://muhammad-fiaz.github.io/FastQueue/guide/scheduler |
| **Sanitizer Support** | Built-in support for AddressSanitizer, UBSan, and ThreadSanitizer. | https://muhammad-fiaz.github.io/FastQueue/guide/installation |
| **Package Managers** | Supports CMake, xmake, Conan, and vcpkg package managers. | https://muhammad-fiaz.github.io/FastQueue/guide/installation |

</details>

----

<details>
<summary><strong>Prerequisites and Supported Platforms</strong> (click to expand)</summary>

<br>

## Prerequisites

Before using `FastQueue`, ensure you have the following:

| Requirement | Version | Notes |
|-------------|---------|-------|
| **C Compiler** | C99 or later (GCC 4.8+, Clang 3.0+, MSVC 2015+) | Supports C99, C11, C17, C23 |
| **CMake** | 3.20+ | Build system |
| **pthreads** | Linux/macOS only | POSIX threads |
| **Win32 API** | Windows only | Thread primitives |

---

## Supported Platforms

`FastQueue` is validated on these architectures:

| Platform | x86_64 (64-bit) | aarch64 (ARM64) |
|----------|-----------------|-----------------|
| **Linux** | Yes | Yes |
| **Windows** | Yes | Yes |
| **macOS** | Yes | Yes (Apple Silicon) |

</details>

---

## Installation

### Method 1: CMake (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(FastQueue
    GIT_REPOSITORY https://github.com/muhammad-fiaz/FastQueue.git
    GIT_TAG main)
FetchContent_MakeAvailable(FastQueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### Method 2: CMake (Subdirectory)

```bash
git clone https://github.com/muhammad-fiaz/FastQueue.git
```

```cmake
add_subdirectory(FastQueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### Method 3: xmake

```lua
add_requires("fastqueue")
target("myapp")
    set_kind("binary")
    add_files("src/main.c")
    add_packages("fastqueue")
```

### Method 4: Conan

```ini
[requires]
fastqueue/0.1.0
```

### Method 5: vcpkg

```bash
vcpkg install fastqueue
```

### Method 6: Build from Source

```bash
git clone https://github.com/muhammad-fiaz/FastQueue.git
cd FastQueue
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build
```

## Quick Start

### Thread Pool

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void my_task(void *arg) {
    printf("Task %d executed\n", *(int *)arg);
}

int main(void) {
    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, 4);

    for (int i = 0; i < 100; ++i) {
        int *id = malloc(sizeof(int));
        *id = i;
        fq_thread_pool_submit_fn(pool, my_task, id);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

### Scheduler with Futures

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

static void compute(void *arg) {
    long *result = (long *)arg;
    *result = (*result) * (*result);
}

int main(void) {
    fq_scheduler_t *scheduler = NULL;
    fq_scheduler_config_t cfg;
    fq_scheduler_config_default(&cfg);
    cfg.thread_count = 4;
    fq_scheduler_create(&scheduler, &cfg);

    long value = 42;
    fq_task_t *task = NULL;
    fq_future_t *future = NULL;
    fq_task_create(&task, compute, &value, NULL);
    fq_scheduler_submit_with_future(scheduler, task, &future);

    fq_future_wait(future);
    printf("42 squared = %ld\n", value);

    fq_future_destroy(future);
    fq_scheduler_shutdown(scheduler);
    return 0;
}
```

### C++ Usage

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>

int main() {
    fq_thread_pool_t *pool = nullptr;
    fq_thread_pool_create_ex(&pool, 4);

    for (int i = 0; i < 50; ++i) {
        int *id = new int(i);
        fq_thread_pool_submit_fn(pool, [](void *arg) {
            std::printf("Task %d\n", *static_cast<int *>(arg));
        }, id);
    }

    fq_thread_pool_wait_idle(pool);
    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Examples

The `examples/` directory contains **6 comprehensive, runnable examples** demonstrating all features:

| Example | Description | Documentation |
|---------|-------------|---------------|
| [`basic_jobs`](examples/basic_jobs.c) | Submit and execute tasks with thread pool | https://muhammad-fiaz.github.io/FastQueue/examples/basic-jobs |
| [`futures`](examples/futures.c) | Awaitable task results | https://muhammad-fiaz.github.io/FastQueue/examples/futures |
| [`parallel_for`](examples/parallel_for.c) | Parallel data processing | https://muhammad-fiaz.github.io/FastQueue/examples/parallel-for |
| [`custom_allocator`](examples/custom_allocator.c) | Pluggable allocator interface | https://muhammad-fiaz.github.io/FastQueue/examples/custom-allocator |
| [`graceful_shutdown`](examples/graceful_shutdown.c) | Clean shutdown patterns | https://muhammad-fiaz.github.io/FastQueue/examples/graceful-shutdown |
| [`error_handling`](examples/error_handling.c) | Error handling best practices | https://muhammad-fiaz.github.io/FastQueue/examples/error-handling |

To run any example:
```bash
cmake -B build -DFQ_BUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/examples/Release/fq_example_basic_jobs
```

## Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `FQ_BUILD_TESTS` | OFF | Build unit tests |
| `FQ_BUILD_EXAMPLES` | OFF | Build examples |
| `FQ_BUILD_BENCHMARKS` | OFF | Build benchmarks |
| `FQ_SHARED` | OFF | Build shared library |
| `FQ_ENABLE_ASAN` | OFF | AddressSanitizer |
| `FQ_ENABLE_UBSAN` | OFF | UndefinedBehaviorSanitizer |
| `FQ_ENABLE_TSAN` | OFF | ThreadSanitizer |

## Performance

Run benchmarks:
```bash
cmake -B build -DFQ_BUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/benchmarks/Release/fq_benchmark
```

### Benchmark Results

Average throughput across multiple runs:

| Configuration | Jobs | Throughput |
|---|---|---|
| 1 thread, 10K jobs | 10,000 | ~1.4M jobs/s |
| 2 threads, 10K jobs | 10,000 | ~1.1M jobs/s |
| 4 threads, 10K jobs | 10,000 | ~1.3M jobs/s |
| 4 threads, 100K jobs | 100,000 | ~1.0M jobs/s |
| 4 threads, 1M jobs | 1,000,000 | ~1.0M jobs/s |

## Testing

Run the full test suite:
```bash
cmake -B build -DFQ_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build --output-on-failure
```

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

MIT License - see [LICENSE](LICENSE) for details.
