---
url: /FastQueue/examples/parallel-for.md
description: Process array elements in parallel using the FastQueue thread pool.
---

# Parallel For Example

Process array elements in parallel across multiple threads.

## C Version

```c
#include <fastqueue/fastqueue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int start;
    int end;
} range_t;

static void square_range(void *arg)
{
    range_t *r = (range_t *)arg;
    for (int i = r->start; i < r->end; ++i) {
        r->data[i] = r->data[i] * r->data[i];
    }
}

int main(void)
{
    const int N = 1000000;
    const int THREADS = 4;
    int *data = malloc(N * sizeof(int));

    for (int i = 0; i < N; ++i) data[i] = i;

    fq_thread_pool_t *pool = NULL;
    fq_thread_pool_create_ex(&pool, THREADS);

    int chunk = N / THREADS;
    range_t ranges[THREADS];

    for (int i = 0; i < THREADS; ++i) {
        ranges[i].data  = data;
        ranges[i].start = i * chunk;
        ranges[i].end   = (i == THREADS - 1) ? N : (i + 1) * chunk;
        fq_thread_pool_submit_fn(pool, square_range, &ranges[i]);
    }

    fq_thread_pool_wait_idle(pool);

    printf("data[0]=%d data[999]=%d data[999999]=%d\n",
           data[0], data[999], data[999999]);

    fq_thread_pool_shutdown(pool);
    free(data);
    return 0;
}
```

## C++ Version

```cpp
#include <fastqueue/fastqueue.h>
#include <cstdio>
#include <vector>
#include <thread>

int main()
{
    const int N = 1000000;
    const int THREADS = 4;
    std::vector<int> data(N);
    for (int i = 0; i < N; ++i) data[i] = i;

    fq_thread_pool_t *pool = nullptr;
    fq_thread_pool_create_ex(&pool, THREADS);

    int chunk = N / THREADS;

    auto square_range = [&data](void *arg) {
        auto *range = static_cast<std::pair<int, int> *>(arg);
        for (int i = range->first; i < range->second; ++i) {
            data[i] = data[i] * data[i];
        }
    };

    std::vector<std::pair<int, int>> ranges(THREADS);
    for (int i = 0; i < THREADS; ++i) {
        ranges[i] = {i * chunk, (i == THREADS - 1) ? N : (i + 1) * chunk};
        fq_thread_pool_submit_fn(pool, square_range, &ranges[i]);
    }

    fq_thread_pool_wait_idle(pool);

    std::printf("data[0]=%d data[999]=%d data[999999]=%d\n",
                data[0], data[999], data[999999]);

    fq_thread_pool_shutdown(pool);
    return 0;
}
```

## Expected Output

```
data[0]=0 data[999]=998001 data[999999]=999998000001
```
