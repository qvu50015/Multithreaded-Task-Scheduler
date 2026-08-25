#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <future>

int main()
{
    ThreadPool pool(1);

    std::vector<std::future<void>> futures;

    futures.push_back(
        pool.enqueue(Priority::LOW, [] {
            std::cout << "Low priority task\n";
        })
    );

    futures.push_back(
        pool.enqueue(Priority::HIGH, [] {
            std::cout << "High priority task\n";
        })
    );

    futures.push_back(
        pool.enqueue(Priority::MEDIUM, [] {
            std::cout << "Medium priority task\n";
        })
    );

    for (auto& future : futures) {
        future.get();
    }

    return 0;
}