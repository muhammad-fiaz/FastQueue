# Installation

## Requirements

| Requirement | Version |
|---|---|
| C compiler | GCC 14+, Clang 18+, MSVC 2022+ |
| CMake | 3.20+ |
| C standard | C23 |

## Build from Source

```bash
git clone https://github.com/fastqueue/fastqueue.git
cd fastqueue
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cmake --install build --prefix /usr/local
```

## CMake Options

| Option | Default | Description |
|---|---|---|
| `FQ_BUILD_TESTS` | `OFF` | Build unit tests |
| `FQ_BUILD_EXAMPLES` | `OFF` | Build example programs |
| `FQ_BUILD_BENCHMARKS` | `OFF` | Build benchmarks |
| `FQ_BUILD_DOCS` | `OFF` | Generate documentation |
| `FQ_SHARED` | `OFF` | Build as shared library |
| `FQ_INSTALL` | `ON` | Generate install targets |
| `FQ_ENABLE_ASAN` | `OFF` | Enable AddressSanitizer |
| `FQ_ENABLE_UBSAN` | `OFF` | Enable UBSan |
| `FQ_ENABLE_TSAN` | `OFF` | Enable ThreadSanitizer |

## Integration

### add_subdirectory

```cmake
add_subdirectory(path/to/fastqueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```

### FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    fastqueue
    GIT_REPOSITORY https://github.com/fastqueue/fastqueue.git
    GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(fastqueue)
target_link_libraries(your_target PRIVATE FastQueue::fastqueue)
```
