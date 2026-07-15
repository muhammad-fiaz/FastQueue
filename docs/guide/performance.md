# Performance

## Benchmarks

Run the included benchmarks:

```bash
cmake -B build -DFQ_BUILD_BENCHMARKS=ON
cmake --build build
./build/benchmarks/fq_benchmark
```

## Tips

1. **Use work stealing** — Enabled by default, improves load balancing.
2. **Batch submissions** — Submit many tasks at once for better throughput.
3. **Tune queue capacity** — Larger queues reduce contention under heavy load.
4. **Match thread count** — Use `fq_hardware_concurrency()` to detect optimal count.
5. **Avoid false sharing** — Worker queues are cache-line padded.

## Cache Line Padding

Worker structs are padded to `FQ_CACHE_LINE_SIZE` (64 bytes) to prevent false sharing between adjacent workers.
