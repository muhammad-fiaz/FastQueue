<div align="center">

# FastQueue

**Production-ready, high-performance Job System & Thread Pool Library in ISO C23.**

A lightweight, zero-dependency C23 library providing work-stealing thread pools, MPMC task queues, futures, and custom allocator support for building high-performance concurrent applications.

[![CI](https://github.com/muhammad-fiaz/FastQueue/actions/workflows/ci.yml/badge.svg)](https://github.com/muhammad-fiaz/FastQueue/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C23-blue.svg)](https://en.wikipedia.org/wiki/C23)
[![Documentation](https://img.shields.io/badge/Docs-VitePress-brightgreen.svg)](https://muhammad-fiaz.github.io/FastQueue/)

</div>

## Features

- **Thread Pool** - Built-in work-stealing thread pool with configurable worker count
- **Job Scheduler** - Priority scheduling with work stealing and statistics
- **Thread-Safe Queue** - MPMC-safe queue with mutex-protected push/pop
- **Futures** - Awaitable results with spin-then-wait strategy
- **Custom Allocators** - Pluggable allocator interface for constrained environments
- **Cross-Platform** - Linux, Windows (MSVC), macOS
- **C23 Modern C** - Forward-compatible with C2Y/C26
- **Zero Dependencies** - Standard library only
- **C++ Compatible** - All headers have `extern "C"` guards

## Quick Start

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

## Building

```bash
git clone https://github.com/muhammad-fiaz/FastQueue.git
cd FastQueue
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run tests:
```bash
cmake -B build -DFQ_BUILD_TESTS=ON -DFQ_BUILD_EXAMPLES=ON
cmake --build build --config Release
./build/tests/Release/fq_tests
```

### CMake Options

| Option | Default | Description |
|---|---|---|
| `FQ_BUILD_TESTS` | OFF | Build unit tests |
| `FQ_BUILD_EXAMPLES` | OFF | Build examples |
| `FQ_BUILD_BENCHMARKS` | OFF | Build benchmarks |
| `FQ_SHARED` | OFF | Build shared library |
| `FQ_ENABLE_ASAN` | OFF | AddressSanitizer |
| `FQ_ENABLE_UBSAN` | OFF | UBSan |
| `FQ_ENABLE_TSAN` | OFF | ThreadSanitizer |

## Integration

### CMake (subdirectory)
```cmake
add_subdirectory(FastQueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### CMake (FetchContent)
```cmake
include(FetchContent)
FetchContent_Declare(FastQueue
    GIT_REPOSITORY https://github.com/muhammad-fiaz/FastQueue.git
    GIT_TAG main)
FetchContent_MakeAvailable(FastQueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### xmake
```lua
add_requires("fastqueue")
target("myapp")
    set_kind("binary")
    add_files("src/main.c")
    add_packages("fastqueue")
```

### Conan
```ini
[requires]
fastqueue/0.1.0
```

### vcpkg
```bash
vcpkg install fastqueue
```

## Documentation

- [Getting Started](https://muhammad-fiaz.github.io/FastQueue/guide/getting-started)
- [API Reference](https://muhammad-fiaz.github.io/FastQueue/api/overview)
- [FAQ](https://muhammad-fiaz.github.io/FastQueue/guide/faq)

## Requirements

- C23 compiler: GCC 14+, Clang 18+, MSVC 2022+
- CMake 3.20+
- pthreads (Linux/macOS) or Win32 threads (Windows)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

MIT License. See [LICENSE](LICENSE) for details.
