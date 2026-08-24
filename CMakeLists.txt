cmake_minimum_required(VERSION 3.20)

project(task_scheduler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main program
add_executable(task_scheduler
    src/main.cpp
    src/ThreadPool.cpp
)

# Tests
add_executable(thread_pool_tests
    tests/ThreadPoolTests.cpp
    src/ThreadPool.cpp
)

# Benchmark
add_executable(benchmark
    benchmarks/Benchmark.cpp
    src/ThreadPool.cpp
)

# Tiny Task Benchmark
add_executable(tiny-task-benchmark
    benchmarks/TinyTaskBenchmark.cpp
    src/ThreadPool.cpp
)

# Uneven Task Benchmark
add_executable(uneven-task-benchmark
    benchmarks/UnevenTaskBenchmark.cpp
    src/ThreadPool.cpp
)

target_include_directories(task_scheduler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(thread_pool_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(tiny-task-benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(uneven-task-benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
