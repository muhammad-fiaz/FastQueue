cmake_minimum_required(VERSION 3.20)

# ── Project ─────────────────────────────────────────────────────────────

project(FastQueue
    VERSION 0.1.0
    DESCRIPTION "Production-ready, high-performance Job System & Thread Pool Library (C99/C11/C17/C23)"
    HOMEPAGE_URL "https://github.com/muhammad-fiaz/FastQueue"
    LANGUAGES C
)

# Allow users to select their preferred C standard (default: C11)
set(FQ_C_STANDARD "11" CACHE STRING "C standard to use (99, 11, 17, or 23)")
set_property(CACHE FQ_C_STANDARD PROPERTY STRINGS "99" "11" "17" "23")

set(CMAKE_C_STANDARD ${FQ_C_STANDARD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Use /Z7 debug format on MSVC to avoid PDB locking in parallel builds
if(MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Z7" CACHE STRING "" FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Z7" CACHE STRING "" FORCE)
endif()

# ── Options ─────────────────────────────────────────────────────────────

option(FQ_BUILD_TESTS       "Build unit tests"            OFF)
option(FQ_BUILD_EXAMPLES    "Build example programs"      OFF)
option(FQ_BUILD_BENCHMARKS  "Build benchmarks"            OFF)
option(FQ_BUILD_DOCS        "Generate documentation"      OFF)
option(FQ_SHARED            "Build as shared library"     OFF)
option(FQ_INSTALL           "Generate install targets"    ON)

# ── Sanitisers ──────────────────────────────────────────────────────────

option(FQ_ENABLE_ASAN       "AddressSanitizer"            OFF)
option(FQ_ENABLE_UBSAN      "UndefinedBehaviorSanitizer"  OFF)
option(FQ_ENABLE_TSAN       "ThreadSanitizer"             OFF)
option(FQ_ENABLE_MSAN       "MemorySanitizer"             OFF)

# ── Compiler warnings ──────────────────────────────────────────────────

if(MSVC)
    add_compile_options(/W4 /WX /wd4100 /wd4189 /wd4505)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror
        -Wconversion -Wsign-conversion -Wshadow
        -Wdouble-promotion -Wformat=2
        -Wno-unused-parameter)
endif()

# ── Sanitiser flags ────────────────────────────────────────────────────

if(FQ_ENABLE_ASAN)
    if(MSVC)
        add_compile_options(/fsanitize=address)
    else()
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
endif()

if(FQ_ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=undefined)
endif()

if(FQ_ENABLE_TSAN)
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
    add_link_options(-fsanitize=thread)
endif()

if(FQ_ENABLE_MSAN)
    add_compile_options(-fsanitize=memory -fno-omit-frame-pointer)
    add_link_options(-fsanitize=memory)
endif()

# ── Sources ─────────────────────────────────────────────────────────────

set(FQ_SOURCES
    src/platform/platform.c
    src/memory/memory.c
    src/queue/queue.c
    src/task/task.c
    src/future/future.c
    src/scheduler/scheduler.c
    src/thread/thread_pool.c
    src/utils/errors.c
    src/utils/version.c
    src/utils/parallel.c
    src/utils/time.c
)

set(FQ_PUBLIC_HEADERS
    include/fastqueue/fastqueue.h
    include/fastqueue/config.h
    include/fastqueue/platform.h
    include/fastqueue/types.h
    include/fastqueue/errors.h
    include/fastqueue/version.h
    include/fastqueue/atomic.h
    include/fastqueue/memory.h
    include/fastqueue/task.h
    include/fastqueue/queue.h
    include/fastqueue/future.h
    include/fastqueue/scheduler.h
    include/fastqueue/thread_pool.h
    include/fastqueue/parallel.h
    include/fastqueue/time.h
)

# ── Library target ──────────────────────────────────────────────────────

if(FQ_SHARED)
    add_library(fastqueue SHARED ${FQ_SOURCES})
    target_compile_definitions(fastqueue PRIVATE FQ_BUILDING_SHARED)
    if(NOT MSVC)
        set_target_properties(fastqueue PROPERTIES
            C_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON)
    endif()
else()
    add_library(fastqueue STATIC ${FQ_SOURCES})
endif()

add_library(FastQueue::fastqueue ALIAS fastqueue)

target_include_directories(fastqueue
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(fastqueue PUBLIC c_std_11)

# Platform-specific link dependencies.
if(WIN32)
    target_link_libraries(fastqueue PRIVATE WinMM)
elseif(APPLE)
    target_link_libraries(fastqueue PRIVATE pthread)
else()
    target_link_libraries(fastqueue PRIVATE pthread rt)
endif()

set_target_properties(fastqueue PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

# ── Install ─────────────────────────────────────────────────────────────

if(FQ_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS fastqueue
        EXPORT FastQueueTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(DIRECTORY include/fastqueue
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h"
    )

    install(EXPORT FastQueueTargets
        FILE FastQueueTargets.cmake
        NAMESPACE FastQueue::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FastQueue
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FastQueueConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/FastQueueConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FastQueue
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/FastQueueConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/FastQueueConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/FastQueueConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FastQueue
    )
endif()

# ── Tests ───────────────────────────────────────────────────────────────

if(FQ_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ── Examples ────────────────────────────────────────────────────────────

if(FQ_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ── Benchmarks ──────────────────────────────────────────────────────────

if(FQ_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
