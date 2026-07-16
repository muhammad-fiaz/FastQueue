cmake_minimum_required(VERSION 3.20)

# ── Tests use CTest + a simple test runner ──────────────────────────────

set(FQ_TEST_SOURCES
    test_main.c
    test_queue.c
    test_task.c
    test_future.c
    test_scheduler.c
    test_memory.c
    test_errors.c
    test_time.c
    test_platform.c
)

add_executable(fq_tests ${FQ_TEST_SOURCES})
target_link_libraries(fq_tests PRIVATE FastQueue::fastqueue)

add_test(NAME FastQueueTests COMMAND fq_tests)
