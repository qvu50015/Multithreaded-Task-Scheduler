# Multithreaded-Task-Scheduler

## Getting Started

How to run

```bash
cmake --build build
./build/task_scheduler
```

## Performance

Benchmarks were run on an Apple Silicon Mac using 1, 2, 4, and 8 workers.
Each configuration was run five times and the average execution time was recorded.

### Prime Counting Benchmark

| Workers | Average Time | Speedup | Efficiency |
|--------:|-------------:|--------:|-----------:|
| Sequential | 740.8 ms | 1.00x | — |
| 1 | 726.2 ms | 1.02x | 100.0% |
| 2 | 379.0 ms | 1.95x | 97.7% |
| 4 | 226.0 ms | 3.28x | 81.9% |
| 8 | 164.2 ms | 4.51x | 56.4% |

Efficiency is calculated as:

`Efficiency = Speedup / Number of Workers × 100`

The scheduler achieved a 4.51x speedup with 8 workers compared with sequential execution. Scaling remained strong through 4 workers, while efficiency decreased at 8 workers due to parallelization and scheduling overhead.

## Tiny-Task Overhead

To measure scheduler overhead, 100,000 extremely small tasks were
submitted to the ThreadPool.

| Configuration | Time |
|---|---:|
| Sequential | 335 µs |
| ThreadPool (1) | 190,164 µs |
| ThreadPool (2) | 280,974 µs |
| ThreadPool (4) | 283,415 µs |
| ThreadPool (8) | 332,152 µs |

The tiny-task workload demonstrates that the ThreadPool introduces
significant overhead when individual tasks perform very little work.
In this case, task creation, synchronization, queue operations, and
future management dominate the cost of the actual computation. 
Tiny-task benchmark timing includes task submission, scheduling, execution, and future result collection, but excludes ThreadPool construction.

### Uneven Workload

| Workers | Average Time | Speedup |
|--------:|-------------:|--------:|
| Sequential | 445 ms | 1.00x |
| 1 | 442.8 ms | 1.00x |
| 2 | 237 ms | 1.88x |
| 4 | 139.4 ms | 3.19x |
| 8 | 99 ms | 4.49x |

### Fine-Grained Tasks

The tiny-task benchmark demonstrated significant scheduler overhead.
100,000 very small tasks took substantially longer through the
thread pool than executing the equivalent computation sequentially.

This demonstrates that task granularity is an important factor in
scheduler performance.