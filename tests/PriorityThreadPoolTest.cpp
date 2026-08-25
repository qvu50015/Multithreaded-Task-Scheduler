#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <future>

int main()
{
    ThreadPool pool(1);

    std::vector<std::future<void>> futures;

    // Low priority tasks
    for (int i = 1; i <= 5; i++) {
        futures.push_back(
            pool.enqueue(Priority::LOW, [i] {
                std::cout << "LOW task " << i << '\n';
            })
        );
    }

    // High priority tasks
    for (int i = 1; i <= 5; i++) {
        futures.push_back(
            pool.enqueue(Priority::HIGH, [i] {
                std::cout << "HIGH task " << i << '\n';
            })
        );
    }

    // Medium priority tasks
    for (int i = 1; i <= 5; i++) {
        futures.push_back(
            pool.enqueue(Priority::MEDIUM, [i] {
                std::cout << "MEDIUM task " << i << '\n';
            })
        );
    }

    for (auto& future : futures) {
        future.get();
    }

    return 0;
}