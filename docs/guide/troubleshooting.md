# Troubleshooting

## Build fails with "C23 not supported"

Ensure your compiler supports C23. Use:
- GCC 14+ (`-std=c23`)
- Clang 18+ (`-std=c23`)
- MSVC 2022+ (`/std:c23`)

## Link errors on Linux

Link against pthreads:

```cmake
target_link_libraries(your_target PRIVATE FastQueue::fastqueue pthread)
```

## ThreadSanitizer reports data races

This is expected with the default spin-then-wait strategy. The spin loop is intentionally racy for performance. Use the condition variable fallback for precise synchronization.

## Tasks seem slow with 1 thread

The scheduler overhead is minimal but measurable. For very short tasks, batch submissions to amortize overhead.

## Future never becomes ready

Ensure the task is actually submitted. A task that fails to submit will never fulfill its future.
