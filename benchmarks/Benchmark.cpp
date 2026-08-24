#include "ThreadPool.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <future>

bool isPrime(int n)
{
    if (n < 2) {
        return false;
    }

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

int countPrimes(int start, int end)
{
    int count = 0;

    for (int i = start; i < end; i++) {
        if (isPrime(i)) {
            count++;
        }
    }

    return count;
}

long long sequentialBenchmark()
{
    int start = 2;
    int end = 5000000;

    auto startTime = std::chrono::steady_clock::now();

    int total = countPrimes(start, end);

    auto endTime = std::chrono::steady_clock::now();

    if (total != 348513) {
        std::cout << "ERROR: Incorrect prime count!\n";
    }

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime
        );

    return duration.count();
}

long long threadPoolBenchmark(int numWorkers)
{
    int start = 2;
    int end = 5000000;
    int numTasks = 16;
    int chunkSize = (end - start) / numTasks;

    auto startTime = std::chrono::steady_clock::now();
    ThreadPool pool(numWorkers);
    std::vector<std::future<int>> futures;

    for (int i = 0; i < numTasks; i++) {
        int rangeStart = start + i * chunkSize;

        int rangeEnd;

        if (i == numTasks - 1) {
            rangeEnd = end;
        }

        else {
            rangeEnd = rangeStart + chunkSize;
        }

        futures.push_back(
            pool.enqueue([rangeStart, rangeEnd] {
                return countPrimes(rangeStart, rangeEnd);
            })
        );
    }

    int total = 0;

    for (auto& future : futures) {
        total += future.get();
    }

    auto endTime = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    if (total != 348513) {
        std::cout << "ERROR: Incorrect prime count!\n";
    }

    return duration.count();
}


int main(){
    std::vector<int> workerCounts = {1, 2, 4, 8};
    long long sequentialTotal = 0;

    for (int run = 0; run < 5; run++) {
        long long time = sequentialBenchmark();
        sequentialTotal += time;

        std::cout << "Sequential | Run: "
                  << run + 1
                  << " | Time: "
                  << time
                  << " ms\n";
    }

    double sequentialAverage = static_cast<double>(sequentialTotal) / 5.0;

    std::cout << "Sequential | Average: " << sequentialAverage << " ms\n\n";

    for (int workers : workerCounts) {
        long long totalTime = 0;

        for (int run = 0; run < 5; run++) {
            long long time = threadPoolBenchmark(workers);
            totalTime += time;
            

            std::cout << "Workers: " << workers << " | Run: "
                    << run + 1
                    << " | Time: "
                    << time
                    << " ms\n";
        }

        double average = static_cast<double>(totalTime) / 5.0;
        
        double speedup = sequentialAverage / average;

        std::cout << "⭐ Workers: " << workers
          << " | Average: " << average
          << " ms"
          << " | Speedup: " << speedup
          << "x\n\n";
        }

    return 0;
}