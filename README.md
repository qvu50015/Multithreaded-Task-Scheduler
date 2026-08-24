# Multithreaded-Task-Scheduler

## Getting Started

How to run

```bash
cmake --build build
./build/task_scheduler
```

## Performance

Benchmarked a CPU-bound prime-counting workload over the range
2–5,000,000 using 16 independent tasks. Each configuration was
run 5 times, with the average execution time reported.

All configurations produced the expected result of 348,513 primes.

| Configuration | Avg Time | Speedup |
|---|---:|---:|
| Sequential | 726.0 ms | 1.00× |
| ThreadPool (1) | 720.2 ms | 1.01× |
| ThreadPool (2) | 381.2 ms | 1.90× |
| ThreadPool (4) | 208.0 ms | 3.49× |
| ThreadPool (8) | 161.6 ms | 4.49× |

The ThreadPool achieved a 4.49× speedup with 8 workers compared
with the sequential baseline. Performance improved as worker
count increased, although scaling diminished at higher worker
counts.