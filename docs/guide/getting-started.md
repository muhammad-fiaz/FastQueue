---
title: Getting Started
description: Learn how to install and set up FastQueue, a high-performance C/C++ thread pool and job system library.
keywords: fastqueue, getting started, install, setup, c, c++, thread pool
---

# Getting Started

FastQueue is a production-ready, high-performance **Job System & Thread Pool Library** written in **C (C99 and later)** with full **C++ compatibility**.

## Features

- Thread pool with work stealing
- Job scheduler with priority support
- Thread-safe MPMC queues
- Futures and promises
- Custom allocators
- Cross-platform (Linux, Windows, macOS)
- Zero external dependencies

## Requirements

- C compiler with C99 or later support (GCC 4.8+, Clang 3.0+, MSVC 2015+)
- CMake 3.20+
- POSIX threads (Linux/macOS) or Win32 threads (Windows)

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

- [Quick Start](/guide/quick-start) - Write your first program
- [Thread Pool](/guide/thread-pool) - Learn about the thread pool
- [API Reference](/api/overview) - Full API docs
