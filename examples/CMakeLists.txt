cmake_minimum_required(VERSION 3.20)

set(FQ_EXAMPLES
    basic_jobs
    futures
    parallel_for
    custom_allocator
    graceful_shutdown
    error_handling
)

foreach(exe ${FQ_EXAMPLES})
    add_executable(fq_example_${exe} ${exe}.c)
    target_link_libraries(fq_example_${exe} PRIVATE FastQueue::fastqueue)
endforeach()
