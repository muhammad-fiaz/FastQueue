---
layout: home
title: FastQueue - High Performance C23 Thread Pool Library
description: Production-ready, high-performance C23 job system and thread pool library with work-stealing scheduler, MPMC queues, futures, and custom allocators.
keywords: c23, thread pool, job system, scheduler, concurrency, work stealing, mpmc queue, futures, parallel, performance

hero:
  name: FastQueue
  text: Production-Ready Job System & Thread Pool
  tagline: High-performance, cross-platform C23 library for concurrent task execution with work-stealing, futures, and custom allocators.
  actions:
    - theme: brand
      text: Get Started
      link: /guide/getting-started
    - theme: alt
      text: API Reference
      link: /api/overview
    - theme: alt
      text: GitHub
      link: https://github.com/muhammad-fiaz/FastQueue

features:
  - title: Work-Stealing Thread Pool
    details: Built-in thread pool with configurable worker count and automatic work stealing across threads for optimal load balancing.
  - title: Job Scheduler
    details: Priority-based job scheduling with statistics, shutdown control, and fine-grained lifecycle management.
  - title: Futures
    details: Awaitable futures with spin-then-wait strategy for ultra-low latency task completion notification.
  - title: MPMC Queue
    details: Thread-safe multi-producer multi-consumer queue with mutex protection for correct concurrent access.
  - title: Custom Allocators
    details: Pluggable allocator interface for memory-constrained and embedded environments.
  - title: Cross-Platform
    details: Runs on Linux, Windows (MSVC), and macOS with zero external dependencies. Standard library only.
  - title: C23 Modern C
    details: Written in ISO C23 with forward compatibility for C2Y/C26. C++ compatible with extern C guards.
  - title: Zero Dependencies
    details: No external libraries required. Uses only the C standard library and platform APIs.
---
