---
title: Errors API
description: FastQueue error codes and error handling API reference.
keywords: errors, error codes, fq_status_t, error handling
---

# Errors API

Error codes and error handling.

## Types

```c
typedef enum fq_status_t {
    FQ_OK           =  0,
    FQ_ERR_INVAL    = -1,
    FQ_ERR_NOMEM    = -2,
    FQ_ERR_BUSY     = -3,
    FQ_ERR_OVERFLOW = -4,
    FQ_ERR_CLOSED   = -5,
    FQ_ERR_TIMEOUT  = -6,
    FQ_ERR_INTERNAL = -7
} fq_status_t;
```

## Functions

```c
const char *fq_error_string(fq_status_t status);
```
Get human-readable error string for a status code.

```c
const char *fq_status_name(fq_status_t status);
```
Get short status name (e.g. "OK", "INVAL", "NOMEM").

## Error Codes

| Code | Name | Description |
|---|---|---|
| 0 | `FQ_OK` | Success |
| -1 | `FQ_ERR_INVAL` | Invalid argument |
| -2 | `FQ_ERR_NOMEM` | Out of memory |
| -3 | `FQ_ERR_BUSY` | Resource busy |
| -4 | `FQ_ERR_OVERFLOW` | Queue overflow |
| -5 | `FQ_ERR_CLOSED` | Resource closed |
| -6 | `FQ_ERR_TIMEOUT` | Operation timed out |
| -7 | `FQ_ERR_INTERNAL` | Internal error |

## Example

```c
fq_status_t st = fq_thread_pool_create_ex(&pool, 4);
if (st != FQ_OK) {
    fprintf(stderr, "Failed to create pool: %s (code=%d)\n",
            fq_error_string(st), st);
    return 1;
}
```
