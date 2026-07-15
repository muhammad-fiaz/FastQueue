# API Overview

All public symbols are prefixed with `fq_` and declared in `<fastqueue/fastqueue.h>`.

## Headers

| Header | Description |
|---|---|
| `fastqueue.h` | Umbrella header — include this for everything |
| `types.h` | Core type definitions |
| `errors.h` | Error codes and handling |
| `version.h` | Version information |
| `config.h` | Compile-time configuration |
| `platform.h` | Platform detection and thread primitives |
| `atomic.h` | Portable atomic operations |
| `memory.h` | Custom allocator interface |
| `task.h` | Task creation and management |
| `queue.h` | Thread-safe queues |
| `future.h` | Futures and promises |
| `scheduler.h` | Job scheduler |
| `thread_pool.h` | High-level thread pool API |

## Naming Conventions

- Types: `fq_<name>_t`
- Functions: `fq_<module>_<verb>()`
- Enums: `fq_<name>_t` with `FQ_<MODULE>_<VALUE>` values
- Macros: `FQ_<NAME>`
- Constants: `FQ_<NAME>`

## Error Handling

All functions return `fq_status_t` (negative = error). Check with `fq_status_ok()`:

```c
fq_status_t st = fq_scheduler_create(&scheduler, &config);
if (!fq_status_ok(st)) {
    fprintf(stderr, "Error: %s\n", fq_status_string(st));
}
```
