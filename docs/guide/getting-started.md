# Getting Started

FastQueue is a production-ready, high-performance **Job System & Thread Pool Library** written in **ISO C23**.

## Features

- Thread pool with work stealing
- Job scheduler with priority support
- Thread-safe queues
- Futures and promises
- Custom allocators
- Cross-platform (Linux, Windows, macOS)
- Zero external dependencies

## Requirements

- C23-capable compiler (GCC 14+, Clang 18+, MSVC 2022+)
- CMake 3.20+
- POSIX threads (Linux/macOS) or Win32 threads (Windows)

## Quick Install

### From source

```bash
git clone https://github.com/fastqueue/fastqueue.git
cd fastqueue
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build
```

### As a subdirectory

```cmake
add_subdirectory(fastqueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### With FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    fastqueue
    GIT_REPOSITORY https://github.com/fastqueue/fastqueue.git
    GIT_TAG main
)
FetchContent_MakeAvailable(fastqueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```
