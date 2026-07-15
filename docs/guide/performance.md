---
title: Performance
description: FastQueue performance characteristics, benchmarking results, and optimization tips.
keywords: performance, benchmark, throughput, latency, optimization
---

# Performance

FastQueue is designed for high-throughput, low-latency task execution.

## Benchmark Results

Measured on a modern x86-64 system:

| Configuration | Jobs | Time | Throughput |
|---|---|---|---|
| 1 thread, 10K jobs | 10,000 | 9.3 ms | ~1.07M jobs/s |
| 2 threads, 10K jobs | 10,000 | 7.4 ms | ~1.35M jobs/s |
| 4 threads, 10K jobs | 10,000 | 5.0 ms | ~2.00M jobs/s |
| 4 threads, 100K jobs | 100,000 | 65.4 ms | ~1.53M jobs/s |
| 4 threads, 1M jobs | 1,000,000 | 677.7 ms | ~1.48M jobs/s |

## Running Benchmarks

```bash
cmake -B build -DFQ_BUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/benchmarks/Release/fq_benchmark
```

## Optimization Tips

### 1. Right-size the Thread Count

```c
// Use hardware concurrency
cfg.thread_count = 0; // Auto-detect

// Or set manually
cfg.thread_count = 8; // Match your CPU cores
```

### 2. Enable Work Stealing

```c
cfg.enable_work_stealing = FQ_TRUE;
```

### 3. Use Appropriate Queue Capacity

```c
// For many small tasks
cfg.queue_capacity = 8192;

// For fewer large tasks
cfg.queue_capacity = 256;
```

### 4. Minimize Lock Contention

Submit tasks from multiple threads to distribute across worker queues:

```c
// Good: Multiple threads submitting
#pragma omp parallel for
for (int i = 0; i < 1000; ++i) {
    fq_thread_pool_submit_fn(pool, task, &data[i]);
}

// Bad: Single thread submitting all
for (int i = 0; i < 1000; ++i) {
    fq_thread_pool_submit_fn(pool, task, &data[i]);
}
```

### 5. Avoid Unnecessary Waiting

```c
// Use futures instead of wait_idle
fq_future_t *future = NULL;
fq_scheduler_submit_with_future(scheduler, task, &future);
// Do other work...
fq_future_wait(future); // Wait only when needed
```

## Profiling

Use your platform's profiler to identify bottlenecks:

```bash
# Linux perf
perf record ./build/benchmarks/Release/fq_benchmark
perf report

# Valgrind
valgrind --tool=callgrind ./build/benchmarks/Release/fq_benchmark

# macOS Instruments
instruments -t "Time Profiler" ./build/benchmarks/Release/fq_benchmark
```
