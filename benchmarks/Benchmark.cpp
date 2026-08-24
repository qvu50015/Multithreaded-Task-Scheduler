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

long long benchmark(int numWorkers)
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

    for (int workers : workerCounts) {
        for (int run = 0; run < 5; run++) {
            long long time = benchmark(workers);

            std::cout << "Workers: " << workers << " | Run: "
                    << run + 1
                    << " | Time: "
                    << time
                    << " ms\n";
        }
    }

    return 0;
}