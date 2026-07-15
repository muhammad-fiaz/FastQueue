# Errors API

Header: `<fastqueue/errors.h>`

## Types

### fq_status_t

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

### fq_status_string

```c
const char *fq_status_string(fq_status_t status);
```

Returns a human-readable string for the status code.

### fq_status_ok

```c
fq_bool_t fq_status_ok(fq_status_t status);
```

Returns `FQ_TRUE` if status is `FQ_OK`.

### fq_last_error

```c
const char *fq_last_error(void);
```

Returns the thread-local error message.

### fq_set_error

```c
void fq_set_error(const char *fmt, ...);
```

Sets the thread-local error message (printf-style).
