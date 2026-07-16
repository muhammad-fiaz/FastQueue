---
url: /FastQueue/api/time.md
description: >-
  FastQueue time utilities API reference for monotonic clocks and
  high-resolution timers.
---

# Time API

Cross-platform monotonic clock and high-resolution timer utilities.

## Types

```c
typedef struct fq_timer_t {
    int64_t start_ns;
} fq_timer_t;
```

High-resolution timer for measuring elapsed time.

## Functions

### Clock

```c
int64_t fq_time_now_ns(void);
```

Returns the current monotonic time in nanoseconds. Suitable for measuring intervals and computing deltas.

### Timer

```c
void fq_timer_start(fq_timer_t *timer);
```

Initialize a timer and record the current time.

```c
int64_t fq_timer_elapsed_ns(const fq_timer_t *timer);
```

Returns elapsed time in nanoseconds since `fq_timer_start` was called.

```c
int64_t fq_timer_elapsed_ms(const fq_timer_t *timer);
```

Returns elapsed time in milliseconds since `fq_timer_start` was called.

## Example

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>

int main(void)
{
    fq_timer_t timer;
    fq_timer_start(&timer);

    /* Do some work */
    for (volatile long i = 0; i < 100000000; i++) {}

    int64_t elapsed_ns = fq_timer_elapsed_ns(&timer);
    int64_t elapsed_ms = fq_timer_elapsed_ms(&timer);

    printf("Elapsed: %lld ns (%lld ms)\n",
           (long long)elapsed_ns, (long long)elapsed_ms);
    return 0;
}
```
