# FAQ

## What C standard is required?

FastQueue is written in **ISO C23**. You need GCC 14+, Clang 18+, or MSVC 2022+.

## Can I use it from C++?

Yes. All public headers include `extern "C"` guards and are compatible with C++17 and later.

## Is it thread-safe?

Yes. All public API functions are thread-safe unless documented otherwise.

## What platforms are supported?

Linux, Windows, and macOS. POSIX threads are used on Linux/macOS; Win32 threads on Windows.

## Can I use a custom allocator?

Yes. See the [Memory Management](/guide/memory) guide.

## How do I enable work stealing?

It is enabled by default. To disable it, set `enable_work_stealing = FQ_FALSE` in the scheduler config.

## Can I cancel pending tasks?

Yes. Call `fq_scheduler_cancel_all()` to cancel all pending tasks.

## What is the maximum number of threads?

`FQ_MAX_THREADS` (default 256). Override via compiler define.
