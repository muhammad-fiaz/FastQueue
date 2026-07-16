---
title: Installation
description: Detailed installation guide for FastQueue C/C++ thread pool library across all platforms and package managers.
keywords: fastqueue install, cmake, xmake, conan, vcpkg, package manager, c++, c
---

# Installation

FastQueue supports multiple installation methods across all platforms.

## Requirements

| Requirement | Minimum Version |
|---|---|
| C Compiler | C99 or later (GCC 4.8+, Clang 3.0+, MSVC 2015+) |
| CMake | 3.20+ |
| pthreads | Linux/macOS only |
| Win32 API | Windows only |

## Package Managers

### CMake (FetchContent)

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

### CMake (subdirectory)

```bash
git clone https://github.com/muhammad-fiaz/FastQueue.git
```

```cmake
add_subdirectory(FastQueue)
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

[generators]
CMakeDeps
CMakeToolchain
```

```bash
conan install . --output-folder=build --build=missing
cmake --preset conan-release
```

### vcpkg

```bash
vcpkg install fastqueue
```

## Building from Source

```bash
git clone https://github.com/muhammad-fiaz/FastQueue.git
cd FastQueue
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### CMake Options

| Option | Default | Description |
|---|---|---|
| `FQ_BUILD_TESTS` | OFF | Build unit tests |
| `FQ_BUILD_EXAMPLES` | OFF | Build examples |
| `FQ_BUILD_BENCHMARKS` | OFF | Build benchmarks |
| `FQ_SHARED` | OFF | Build shared library |
| `FQ_INSTALL` | ON | Generate install targets |
| `FQ_ENABLE_ASAN` | OFF | AddressSanitizer |
| `FQ_ENABLE_UBSAN` | OFF | UndefinedBehaviorSanitizer |
| `FQ_ENABLE_TSAN` | OFF | ThreadSanitizer |

### Example Build Commands

```bash
# Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Debug build with tests and examples
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DFQ_BUILD_TESTS=ON -DFQ_BUILD_EXAMPLES=ON
cmake --build build --config Debug

# Run tests
./build/tests/Release/fq_tests

# Run benchmark
./build/benchmarks/Release/fq_benchmark
```

## Platform-Specific Notes

### Linux

Requires pthreads (usually available by default):

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Windows (MSVC)

```cmd
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### macOS

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
