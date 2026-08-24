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

long long threadPoolBenchmark(int numWorkers)
{
    const int numTasks = 100000;

    auto start = std::chrono::steady_clock::now();

    ThreadPool pool(numWorkers);

    std::vector<std::future<long long>> futures;

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
    long long sequentialTime = sequentialBenchmark();

    std::cout << "Sequential: " << sequentialTime << " µs\n\n";

    std::vector<int> workerCounts = {1, 2, 4, 8};

    for (int workers : workerCounts) {
        long long time = threadPoolBenchmark(workers);

        std::cout << "Workers: " << workers << " | Time: " << time << " µs\n";
    }

    return 0;
}