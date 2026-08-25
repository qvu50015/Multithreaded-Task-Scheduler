#include "ThreadPool.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <future>

long long tinyWork(int value){
    return value * 2LL;
}

long long sequentialBenchmark(){
    long long total = 0;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < 100000; i++) {
        total += tinyWork(i);
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Sequential result: " << total << '\n';

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );

    return duration.count();
}

long long threadPoolBenchmark(int numWorkers){
    const int numTasks = 100000;

    ThreadPool pool(numWorkers);
    std::vector<std::future<long long>> futures;
    
    // Measure task submission, scheduling, execution, and result collection.
    // ThreadPool construction is excluded.
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < numTasks; i++) {
        futures.push_back(
            pool.enqueue([i] {
                return tinyWork(i);
            })
        );
    }

    long long total = 0;

    for (auto& future : futures) {
        total += future.get();
    }

    auto end = std::chrono::steady_clock::now();

    if (total != 9999900000LL) {
        std::cout << "ERROR: Incorrect result!\n";
    }

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );

    return duration.count();
}

int main()
{
    const int numRuns = 5;
    std::vector<int> workerCounts = {1, 2, 4, 8};

    // Sequential benchmark
    long long sequentialTotal = 0;

    for (int run = 0; run < numRuns; run++) {
        long long time = sequentialBenchmark();

        sequentialTotal += time;

        std::cout << "Sequential | Run: "
                  << run + 1
                  << " | Time: "
                  << time
                  << " µs\n";
    }

    double sequentialAverage =
        static_cast<double>(sequentialTotal) / numRuns;

    std::cout << "Sequential | Average: "
              << sequentialAverage
              << " µs\n\n";


    // ThreadPool benchmark
    for (int workers : workerCounts) {

        long long totalTime = 0;

        for (int run = 0; run < numRuns; run++) {

            long long time = threadPoolBenchmark(workers);

            totalTime += time;

            std::cout << "Workers: "
                      << workers
                      << " | Run: "
                      << run + 1
                      << " | Time: "
                      << time
                      << " µs\n";
        }

        double average =
            static_cast<double>(totalTime) / numRuns;

        double slowdown =
            average / sequentialAverage;

        std::cout << "⭐ Workers: "
                  << workers
                  << " | Average: "
                  << average
                  << " µs"
                  << " | Slowdown: "
                  << slowdown
                  << "x\n\n";
    }

    return 0;
}