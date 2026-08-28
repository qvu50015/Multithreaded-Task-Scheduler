#include "ThreadPool.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <future>
#include <thread>

void expensiveWork(int milliseconds)
{
    auto start = std::chrono::steady_clock::now();

    while (true) {
        auto now = std::chrono::steady_clock::now();

        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - start
            ).count();

        if (elapsed >= milliseconds) {
            break;
        }
    }
}

int main()
{
    const int numWorkers = 4;
    const int numTasks = 16;

    ThreadPool pool(numWorkers);

    std::vector<std::future<void>> futures;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < numTasks; i++) {

        int workload;

        if (i < 4) {
            workload = 1000;
        }
        else {
            workload = 100;
        }

        futures.push_back(
            pool.enqueue([i, workload] {

                auto taskStart = std::chrono::steady_clock::now();

                expensiveWork(workload);

                auto taskEnd = std::chrono::steady_clock::now();

                auto duration =
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        taskEnd - taskStart
                    ).count();

                std::cout
                    << "Task " << i
                    << " | Worker " << getCurrentWorkerId()
                    << " | Work: " << workload << " ms"
                    << " | Actual: " << duration << " ms\n";
            })
        );
    }

    for (auto& future : futures) {
        future.get();
    }

    auto end = std::chrono::steady_clock::now();

    auto totalTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start
        ).count();

    std::cout << "\nTotal execution time: "
              << totalTime
              << " ms\n";

    std::cout << "Work stealing benchmark completed.\n";

    return 0;
}