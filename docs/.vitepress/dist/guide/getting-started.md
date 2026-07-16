---
url: /FastQueue/guide/getting-started.md
description: >-
  Learn how to install and set up FastQueue, a high-performance C23 thread pool
  and job system library.
---

# Getting Started

FastQueue is a production-ready, high-performance **Job System & Thread Pool Library** written in **ISO C23**.

## Features

* Thread pool with work stealing
* Job scheduler with priority support
* Thread-safe MPMC queues
* Futures and promises
* Custom allocators
* Cross-platform (Linux, Windows, macOS)
* Zero external dependencies

## Requirements

* C23-capable compiler (GCC 14+, Clang 18+, MSVC 2022+)
* CMake 3.20+
* POSIX threads (Linux/macOS) or Win32 threads (Windows)

## Quick Install

### From source

```bash
git clone https://github.com/muhammad-fiaz/FastQueue.git
cd FastQueue
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build
```

### As a subdirectory

```cmake
add_subdirectory(FastQueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### With FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    FastQueue
    GIT_REPOSITORY https://github.com/muhammad-fiaz/FastQueue.git
    GIT_TAG main
)
FetchContent_MakeAvailable(FastQueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### With xmake

```bash
xmake repo add fastqueue https://github.com/muhammad-fiaz/FastQueue
xmake require fastqueue
```

### With Conan

```ini
[requires]
fastqueue/0.1.0
```

### With vcpkg

```bash
vcpkg install fastqueue
```

## Next Steps

* [Quick Start](/guide/quick-start) - Write your first program
* [Thread Pool](/guide/thread-pool) - Learn about the thread pool
* [API Reference](/api/overview) - Full API docs
