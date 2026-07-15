/**
 * @file fastqueue.h
 * @brief Main umbrella header for the FastQueue library.
 *
 * Include this single header to access the entire public API.
 *
 * @copyright SPDX-License-Identifier: MIT
 * @author FastQueue Contributors
 * @version 0.1.0
 * @date 2026
 */

#ifndef FASTQUEUE_H
#define FASTQUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fastqueue/config.h"
#include "fastqueue/platform.h"
#include "fastqueue/types.h"
#include "fastqueue/errors.h"
#include "fastqueue/version.h"
#include "fastqueue/atomic.h"
#include "fastqueue/memory.h"
#include "fastqueue/task.h"
#include "fastqueue/queue.h"
#include "fastqueue/future.h"
#include "fastqueue/scheduler.h"
#include "fastqueue/thread_pool.h"
#include "fastqueue/parallel.h"
#include "fastqueue/time.h"

#ifdef __cplusplus
}
#endif

#endif /* FASTQUEUE_H */
