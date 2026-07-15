---
title: Examples
description: Complete collection of FastQueue examples for all features.
keywords: examples, code samples, tutorials
---

# Examples

All FastQueue examples with complete, runnable code.

## Thread Pool

| Example | Description |
|---|---|
| [Basic Jobs](/FastQueue/examples/basic-jobs) | Submit and execute simple tasks |
| [Parallel For](/FastQueue/examples/parallel-for) | Parallel data processing |

## Scheduler

| Example | Description |
|---|---|
| [Scheduler Basics](/FastQueue/examples/basic-jobs) | Low-level scheduler usage |
| [Work Stealing](/FastQueue/examples/basic-jobs) | Demonstrate work stealing |

## Futures

| Example | Description |
|---|---|
| [Futures](/FastQueue/examples/futures) | Awaitable task results |
| [Multiple Futures](/FastQueue/examples/futures) | Wait for multiple tasks |

## Memory

| Example | Description |
|---|---|
| [Custom Allocator](/FastQueue/examples/custom-allocator) | Pluggable allocator interface |

## Patterns

| Example | Description |
|---|---|
| [Graceful Shutdown](/FastQueue/examples/graceful-shutdown) | Clean shutdown patterns |
| [Error Handling](/FastQueue/examples/error-handling) | Error handling best practices |

## C++ Usage

All examples work in C++ with `extern "C"` compatibility:

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>

int main()
{
    fq_thread_pool_t *pool = nullptr;
    fq_thread_pool_create_ex(&pool, 4);
    // ... same API as C
    fq_thread_pool_shutdown(pool);
    return 0;
}
```
