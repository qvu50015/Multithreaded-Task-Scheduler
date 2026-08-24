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

int main()
{
    long long time = sequentialBenchmark();

    std::cout << "Sequential: " << time << " µs\n";

    return 0;
}