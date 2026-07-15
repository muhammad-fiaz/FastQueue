---
title: Config API
description: FastQueue configuration constants and build options reference.
keywords: config, configuration, constants, build options
---

# Config API

Build-time configuration constants.

## Constants

```c
// Default thread count (auto-detect)
FQ_DEFAULT_THREADS

// Default log level
FQ_DEFAULT_LOG_LEVEL

// Queue defaults
FQ_QUEUE_DEFAULT_CAPACITY
FQ_QUEUE_MIN_CAPACITY
FQ_QUEUE_MAX_CAPACITY

// Thread limits
FQ_MAX_THREADS

// Future spin count before falling back to condition variable
FQ_FUTURE_SPIN_COUNT

// Cache-line size in bytes for padding
FQ_CACHE_LINE_SIZE
```

## Log Levels

```c
typedef enum fq_log_level_t {
    FQ_LOG_NONE   = 0,
    FQ_LOG_ERROR  = 1,
    FQ_LOG_WARN   = 2,
    FQ_LOG_INFO   = 3,
    FQ_LOG_DEBUG  = 4
} fq_log_level_t;
```

## Log Callback

```c
typedef void (*fq_log_fn)(fq_log_level_t level, const char *message, void *ctx);
```

## Example

```c
void my_logger(fq_log_level_t level, const char *msg, void *ctx)
{
    printf("[%d] %s\n", level, msg);
}

fq_scheduler_config_t cfg;
fq_scheduler_config_default(&cfg);
cfg.log_callback = my_logger;
cfg.log_level    = FQ_LOG_DEBUG;
cfg.log_context  = NULL;
```
