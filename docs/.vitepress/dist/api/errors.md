---
url: /FastQueue/api/errors.md
description: FastQueue error codes and error handling API reference.
---

# Errors API

Error codes and error handling.

## Types

```c
typedef enum fq_status_t {
    FQ_OK            =  0,
    FQ_ERR_NOMEM     = -1,
    FQ_ERR_INVAL     = -2,
    FQ_ERR_BUSY      = -3,
    FQ_ERR_TIMEOUT   = -4,
    FQ_ERR_CANCELED  = -5,
    FQ_ERR_OVERFLOW  = -6,
    FQ_ERR_CLOSED    = -7,
    FQ_ERR_INTERNAL  = -8,
    FQ_ERR_NOSUPPORT = -9
} fq_status_t;
```

## Functions

```c
const char *fq_status_string(fq_status_t status);
```

Get human-readable error string for a status code.

```c
fq_bool_t fq_status_ok(fq_status_t status);
```

Check whether a status code indicates success (`FQ_TRUE` if `FQ_OK`).

```c
const char *fq_last_error(void);
```

Get the thread-local error message from the last failed operation. Returns an empty string if no error.

```c
void fq_set_error(const char *fmt, ...);
```

Set the thread-local error message with a printf-style format string.

## Error Codes

| Code | Name | Description |
|---|---|---|
| 0 | `FQ_OK` | Success |
| -1 | `FQ_ERR_NOMEM` | Out of memory |
| -2 | `FQ_ERR_INVAL` | Invalid argument |
| -3 | `FQ_ERR_BUSY` | Resource busy / would block |
| -4 | `FQ_ERR_TIMEOUT` | Operation timed out |
| -5 | `FQ_ERR_CANCELED` | Operation was canceled |
| -6 | `FQ_ERR_OVERFLOW` | Queue is full |
| -7 | `FQ_ERR_CLOSED` | System has been shut down |
| -8 | `FQ_ERR_INTERNAL` | Internal error |
| -9 | `FQ_ERR_NOSUPPORT` | Operation not supported |

## Example

```c
fq_status_t st = fq_thread_pool_create_ex(&pool, 4);
if (!fq_status_ok(st)) {
    fprintf(stderr, "Failed to create pool: %s (code=%d)\n",
            fq_status_string(st), st);
    return 1;
}
```
