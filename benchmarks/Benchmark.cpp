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

int main()
{
    int start = 2;
    int end = 5000000;
    int numTasks = 16;
    int chunkSize = (end - start) / numTasks;

    auto startTime = std::chrono::steady_clock::now();

    ThreadPool pool(1);

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

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime
        );

    std::cout << "Primes: " << total << '\n';
    std::cout << "ThreadPool: " << duration.count() << " ms\n";

    return 0;
}