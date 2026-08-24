#include "ThreadPool.h"
#include <future>
#include <iostream>
#include <chrono>
#include <vector>

long long unevenWork(int workload){
    long long result = 0;

    for (int i = 0; i < workload; i++) {
        result += i;
    }

    return result;
}

long long sequentialBenchmark(){
    std::vector<int> workloads = {
        1000000,
        50000000,
        2000000,
        80000000,
        1000000,
        60000000,
        3000000,
        70000000,
        2000000,
        50000000,
        1000000,
        90000000,
        3000000,
        60000000,
        2000000,
        80000000
    };

    long long total = 0;

    auto start = std::chrono::steady_clock::now();

    for (int workload : workloads) {
        total += unevenWork(workload);
    }

    auto end = std::chrono::steady_clock::now();

    std::cout << "Sequential result: " << total << '\n';

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start
        );

    return duration.count();
}

long long threadPoolBenchmark(int numWorkers)
{
    std::vector<int> workloads = {
        1000000,
        50000000,
        2000000,
        80000000,
        1000000,
        60000000,
        3000000,
        70000000,
        2000000,
        50000000,
        1000000,
        90000000,
        3000000,
        60000000,
        2000000,
        80000000
    };

    ThreadPool pool(numWorkers);

    std::vector<std::future<long long>> futures;

    auto start = std::chrono::steady_clock::now();

    for (int workload : workloads) {
        futures.push_back(
            pool.enqueue([workload] {
                return unevenWork(workload);
            })
        );
    }

    long long total = 0;

    for (auto& future : futures) {
        total += future.get();
    }

    if (total != 19016499722500000LL) {
        std::cout << "ERROR: Incorrect result!\n";
    }

    auto end = std::chrono::steady_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
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
                  << " ms\n";
    }

    double sequentialAverage =
        static_cast<double>(sequentialTotal) / numRuns;

    std::cout << "Sequential | Average: "
              << sequentialAverage
              << " ms\n\n";


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
                      << " ms\n";
        }

        double average =
            static_cast<double>(totalTime) / numRuns;

        double speedup = sequentialAverage / average;

        std::cout << "Workers: "
                  << workers
                  << " | Average: "
                  << average
                  << " ms"
                  << " | Speedup: "
                  << speedup
                  << "x\n\n";
    }

    return 0;
}