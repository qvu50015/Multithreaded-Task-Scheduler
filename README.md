# Multithreaded Task Scheduler

A high-performance multithreaded task scheduler written in **C++20**.

The project explores thread pools, per-worker task queues, work stealing, futures, synchronization, concurrent data structures, and performance analysis across different workload types.

## Features

* C++20 thread pool
* Persistent worker threads
* Per-worker task queues
* Work stealing for load balancing
* `std::future`-based task results
* `std::packaged_task` task execution
* Condition-variable-based worker sleeping
* Graceful thread-pool shutdown
* Priority queue implementation
* CPU scaling benchmarks
* Uneven-workload benchmarks
* Fine-grained task overhead analysis
* Experimental atomic queue implementation
* Experimental atomic work-stealing deque
* Concurrent stress tests with up to 100,000 tasks

## Project Structure

```text
Multithreaded-Task-Scheduler/
│
├── benchmarks/
│   ├── results/
│   ├── Benchmark.cpp
│   ├── TinyTaskBenchmark.cpp
│   ├── UnevenTaskBenchmark.cpp
│   └── WorkStealingBenchmark.cpp
│
├── include/
│   ├── LockFreeTaskQueue.h
│   ├── TaskQueue.h
│   ├── ThreadPool.h
│   └── WorkStealingDeque.h
|
├── src/
│   ├── LockFreeTaskQueue.cpp
│   ├── main.cpp
│   ├── TaskQueue.cpp
│   ├── ThreadPool.cpp
│   └── WorkStealingDeque.cpp
|
├── tests/
│   ├── LockFreeTaskQueue.cpp
│   ├── TaskQueueStealTest.cpp
│   ├── TaskQueueTest.cpp
│   ├── ThreadPoolTest.cpp
│   ├── WorkerDistributionTest.cpp
│   ├── WorkStealingDequeTest.cpp
│   └── WorkStealingTest.cpp
│
├── CMakeLists.txt
└── README.md
```

## Architecture

Tasks submitted to the thread pool are distributed across per-worker queues.

Each worker first attempts to execute work from its own queue. If its queue is empty, it attempts to steal work from another worker's queue.

```text
                 ThreadPool
                     |
        +------------+------------+
        |            |            |
        v            v            v
    Worker 1     Worker 2     Worker 3 ...
      Queue         Queue         Queue
        |            |            |
        +------ Work Stealing ----+
```

The production scheduler uses mutex-protected per-worker queues for predictable synchronization and correctness.

## Getting Started

Build the project:

```bash
cmake -S . -B build
cmake --build build
```

### Quick Demo

Run the example program:

```bash
./build/task_scheduler
```

This demonstrates task submission, `std::future` results, multiple return types, and exception propagation.

Run the work-stealing demo:

```bash
./build/work_stealing_test
```

This demonstrates tasks being executed across multiple workers with work stealing.

### Benchmarks

Run the CPU-bound prime-counting benchmark:

```bash
./build/benchmark
```

Run the uneven-workload benchmark:

```bash
./build/uneven-task-benchmark
```

Run the tiny-task overhead benchmark:

```bash
./build/tiny-task-benchmark
```

### Correctness and Stress Tests

```bash
./build/thread_pool_tests
./build/work_stealing_deque_test
./build/lock_free_queue_test
```


## Performance

Benchmarks were run on an **Apple Silicon Mac** using 1, 2, 4, and 8 worker threads.

Each configuration was run **10 times**, and the average execution time was recorded.

### Prime Counting Benchmark

The CPU-intensive benchmark counts prime numbers from 2 to 5,000,000 using 16 tasks.

|    Workers | Average Time |   Speedup | Efficiency |
| ---------: | -----------: | --------: | ---------: |
| Sequential |     720.4 ms |     1.00x |          — |
|          1 |     720.1 ms |     1.00x |      ~100% |
|          2 |     387.0 ms |     1.86x |      93.1% |
|          4 |     212.3 ms |     3.39x |      84.8% |
|          8 |     161.5 ms | **4.46x** |      55.8% |

Efficiency is calculated as:

`Efficiency = Speedup / Number of Workers × 100`

The scheduler achieved a **4.46x speedup with 8 workers** compared with sequential execution.

Scaling remained strong through four workers. At eight workers, execution time continued to improve, while efficiency decreased because synchronization, scheduling overhead, contention, and available CPU parallelism limit ideal linear scaling.

### Uneven Workload Benchmark

This benchmark uses 16 tasks with deliberately uneven computational workloads ranging from 1,000,000 to 90,000,000 loop iterations.

|    Workers | Average Time |   Speedup |
| ---------: | -----------: | --------: |
| Sequential |     444.4 ms |     1.00x |
|          1 |     443.8 ms |     1.00x |
|          2 |     237.9 ms |     1.87x |
|          4 |     133.8 ms |     3.32x |
|          8 |     100.5 ms | **4.42x** |

The scheduler maintained strong scaling under uneven task sizes, reaching a **4.42x speedup with 8 workers**.

Per-worker instrumentation was used to record task counts, computational workload, and execution time across workers.

Because workers can steal tasks from other queues after finishing their local work, the scheduler can dynamically redistribute work instead of leaving workers idle while another worker remains overloaded.

### Tiny-Task Overhead

To measure scheduling overhead, 100,000 extremely small tasks were submitted to the thread pool.

Each task performs only a simple multiplication.

| Configuration          | Average Time | Slowdown vs. Sequential |
| ---------------------- | -----------: | ----------------------: |
| Sequential             |     336.1 µs |                   1.00x |
| ThreadPool (1 worker)  |   111,141 µs |                  330.7x |
| ThreadPool (2 workers) |   111,844 µs |                  332.8x |
| ThreadPool (4 workers) |   112,702 µs |                  335.3x |
| ThreadPool (8 workers) |   108,909 µs |                  324.0x |

For extremely fine-grained tasks, scheduler overhead is much larger than the computation itself.

Task creation, futures, synchronization, queue operations, scheduling, and result collection dominate the cost of executing each tiny task.

This demonstrates an important task-scheduling tradeoff: **parallel execution is most effective when individual tasks contain enough useful computation to amortize scheduling overhead.**

Tiny-task timing includes task submission, scheduling, execution, and future result collection, but excludes construction of the `ThreadPool`.

## Work Stealing

Each worker primarily executes tasks from its own queue.

When a worker becomes idle, it checks other workers' queues and attempts to steal available work.

```text
Worker 1: [Task][Task][Task][Task]
Worker 2: [Task]
Worker 3: []
Worker 4: []

                 |
                 v

Idle workers steal available work from
workers that still have queued tasks.
```

This helps reduce idle time when task execution costs are uneven.

## Experimental Concurrent Data Structures

The project also includes standalone concurrent-data-structure experiments used to explore advanced C++ synchronization concepts.

These implementations explore:

* `std::atomic`
* compare-and-swap operations
* explicit C++ memory ordering
* lock-free-style linked structures
* owner-pop / thief-steal work-stealing semantics
* concurrent stress testing

### Atomic Queue Prototype

A standalone atomic queue prototype successfully processed **100,000 tasks** under concurrent access.

### Experimental Work-Stealing Deque

The experimental work-stealing deque uses separate logical ends for the owning worker and stealing threads.

The owner removes tasks from one end while thief threads attempt to claim tasks from the opposite end.

It was stress-tested with one owner and multiple thief threads:

| Metric          |  Result |
| --------------- | ------: |
| Expected Tasks  | 100,000 |
| Completed Tasks | 100,000 |
| Missing Tasks   |       0 |
| Duplicate Tasks |       0 |

The production `ThreadPool` continues to use mutex-backed per-worker task queues.

The atomic queue and work-stealing deque are maintained as experimental components for studying compare-and-swap operations, memory ordering, and concurrent queue design.

## Performance Findings

The benchmarks demonstrate several important characteristics of multithreaded scheduling:

* CPU-intensive workloads scale effectively across multiple workers.
* The prime-counting workload reached **4.46x speedup with 8 workers**.
* The uneven workload reached **4.42x speedup with 8 workers**.
* Parallel efficiency decreases as worker count increases.
* Work stealing provides a mechanism for idle workers to claim queued work from other workers when local work is exhausted.
* Extremely small tasks perform poorly because scheduler overhead dominates computation.
* Task granularity is an important factor in determining whether parallel execution is beneficial.

## Technologies

* C++20
* `std::thread`
* `std::atomic`
* `std::mutex`
* `std::condition_variable`
* `std::future`
* `std::packaged_task`
* `std::function`
* CMake

## Design Decisions

### Per-Worker Queues

Using one queue per worker reduces contention compared with having every worker compete for a single global queue.

### Work Stealing

Workers first process their own local tasks. When a local queue becomes empty, the worker attempts to steal work from another queue.

This provides dynamic load balancing for workloads where task execution times differ significantly.

### Mutex-Backed Production Queues

The production scheduler uses mutex-protected task queues because they provide straightforward ownership semantics and predictable correctness.

The project also explores atomic queue designs separately without making experimental concurrent data structures part of the primary scheduler path.

### Futures

Tasks are wrapped in `std::packaged_task`, allowing callers to receive results and exceptions through `std::future`.

### Condition Variables

Workers use a condition variable instead of continuously polling for work, reducing unnecessary CPU usage while idle.

## Future Work

Potential extensions include:

* Task cancellation
* Task dependency graphs