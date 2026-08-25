cmake_minimum_required(VERSION 3.20)

project(task_scheduler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main program
add_executable(task_scheduler
    src/main.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
)

# Tests
add_executable(thread_pool_tests
    tests/ThreadPoolTests.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
)

# Benchmark
add_executable(benchmark
    benchmarks/Benchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
)

# Tiny Task Benchmark
add_executable(tiny-task-benchmark
    benchmarks/TinyTaskBenchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
)

# Uneven Task Benchmark
add_executable(uneven-task-benchmark
    benchmarks/UnevenTaskBenchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
)

# Task Queue test
add_executable(task_queue_test
    tests/TaskQueueTest.cpp
    src/TaskQueue.cpp
    src/ThreadPool.cpp
)

# Priority Queue Test
add_executable(priority_queue_test
    tests/PriorityQueueTest.cpp
    src/PriorityQueue.cpp
)

# # Priority Thread Pool Test
# add_executable(priority_thread_pool_test
#     tests/PriorityThreadPoolTest.cpp
#     src/ThreadPool.cpp
#     src/PriorityQueue.cpp
# )

add_executable(worker_distribution_test
    tests/WorkerDistributionTest.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
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

target_include_directories(task_queue_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(priority_queue_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# target_include_directories(priority_thread_pool_test PRIVATE
#     ${CMAKE_SOURCE_DIR}/include
# )

target_include_directories(worker_distribution_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
