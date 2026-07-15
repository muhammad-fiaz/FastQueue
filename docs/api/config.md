# Config API

Header: `<fastqueue/config.h>`

## Compile-Time Options

All options can be overridden via `-D` compiler flags.

| Macro | Default | Description |
|---|---|---|
| `FQ_MAX_THREADS` | 256 | Maximum worker threads |
| `FQ_DEFAULT_THREADS` | 0 (auto) | Default thread count |
| `FQ_QUEUE_MIN_CAPACITY` | 64 | Minimum queue capacity |
| `FQ_QUEUE_DEFAULT_CAPACITY` | 1024 | Default queue capacity |
| `FQ_QUEUE_MAX_CAPACITY` | 16M | Maximum queue capacity |
| `FQ_FUTURE_SPIN_COUNT` | 128 | Spin iterations before wait |
| `FQ_SHUTDOWN_TIMEOUT_MS` | 5000 | Default shutdown timeout |
| `FQ_CACHE_LINE_SIZE` | 64 | Cache line padding size |
| `FQ_DEFAULT_LOG_LEVEL` | INFO (debug) / WARN (release) | Minimum log level |

## Log Levels

| Level | Value | Description |
|---|---|---|
| `FQ_LOG_LEVEL_NONE` | 0 | No logging |
| `FQ_LOG_LEVEL_ERROR` | 1 | Errors only |
| `FQ_LOG_LEVEL_WARN` | 2 | Warnings and errors |
| `FQ_LOG_LEVEL_INFO` | 3 | Info, warnings, errors |
| `FQ_LOG_LEVEL_DEBUG` | 4 | All messages |
