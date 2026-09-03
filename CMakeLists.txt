cmake_minimum_required(VERSION 3.20)

project(task_scheduler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main program
add_executable(task_scheduler
    src/main.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/LockFreeTaskQueue.cpp
    src/WorkStealingDeque.cpp
)

# Tests
add_executable(thread_pool_tests
    tests/ThreadPoolTests.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

# Benchmark
add_executable(benchmark
    benchmarks/Benchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

# Tiny Task Benchmark
add_executable(tiny-task-benchmark
    benchmarks/TinyTaskBenchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

# Uneven Task Benchmark
add_executable(uneven-task-benchmark
    benchmarks/UnevenTaskBenchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

# Work stealing Benchmark
add_executable(work_stealing_benchmark
    benchmarks/WorkStealingBenchmark.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

# Task Queue test
add_executable(task_queue_test
    tests/TaskQueueTest.cpp
    src/TaskQueue.cpp
    src/ThreadPool.cpp
    src/WorkStealingDeque.cpp
)

add_executable(worker_distribution_test
    tests/WorkerDistributionTest.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

add_executable(task_queue_steal_test
    tests/TaskQueueStealTest.cpp
    src/TaskQueue.cpp
)

add_executable(work_stealing_test
    tests/WorkStealingTest.cpp
    src/ThreadPool.cpp
    src/TaskQueue.cpp
    src/WorkStealingDeque.cpp
)

add_executable(lock_free_queue_test
    tests/LockFreeTaskQueueTest.cpp
    src/LockFreeTaskQueue.cpp
)

add_executable(work_stealing_deque_test
    tests/WorkStealingDequeTest.cpp
    src/WorkStealingDeque.cpp
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

target_include_directories(worker_distribution_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(task_queue_steal_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(work_stealing_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(work_stealing_benchmark PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(lock_free_queue_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(work_stealing_deque_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
