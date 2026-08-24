#include "ThreadPool.h"
#include <iostream>
#include <chrono>

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

int main(){
    int start = 2;
    int end = 5000000;
    int chunkSize = (end - start) / 4;

    int range1Start = start;
    int range1End = start + chunkSize;

    int range2Start = range1End;
    int range2End = range2Start + chunkSize;

    int range3Start = range2End;
    int range3End = range3Start + chunkSize;

    int range4Start = range3End;
    int range4End = end;

    auto startTime = std::chrono::steady_clock::now();
    ThreadPool pool(8);

    auto future1 = pool.enqueue([range1Start, range1End] {
        return countPrimes(range1Start, range1End);
    });

    auto future2 = pool.enqueue([range2Start, range2End] {
        return countPrimes(range2Start, range2End);
    });

    auto future3 = pool.enqueue([range3Start, range3End] {
        return countPrimes(range3Start, range3End);
    });

    auto future4 = pool.enqueue([range4Start, range4End] {
        return countPrimes(range4Start, range4End);
    });

    int result1 = future1.get();
    int result2 = future2.get();
    int result3 = future3.get();
    int result4 = future4.get();

    int total = result1 + result2 + result3 + result4;

    auto endTime = std::chrono::steady_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            endTime - startTime
        );

    std::cout << "Primes: " << total << '\n';
    std::cout << "ThreadPool [8]: " << duration.count() << " ms\n";

    return 0;
}