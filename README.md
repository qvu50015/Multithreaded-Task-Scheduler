# Multithreaded-Task-Scheduler

## Getting Started

How to run

```bash
cmake --build build
./build/task_scheduler
```

## Performance

CPU-bound prime-counting benchmark over the range 2–5,000,000
using 16 independent tasks. Each configuration was run five times.

| Configuration | Average Time | Speedup |
|---|---:|---:|
| Sequential | 730.6 ms | 1.00× |
| ThreadPool (1 worker) | 726.0 ms | 1.01× |
| ThreadPool (2 workers) | 395.4 ms | 1.85× |
| ThreadPool (4 workers) | 220.8 ms | 3.31× |
| ThreadPool (8 workers) | 168.2 ms | 4.34× |

All configurations produced the same result: 348,513 primes.