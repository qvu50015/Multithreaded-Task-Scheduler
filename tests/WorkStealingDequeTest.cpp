#include "WorkStealingDeque.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main()
{
    constexpr int numTasks = 100000;
    constexpr int numThieves = 3;

    // Capacity must be large enough because this version
    // of the deque does not resize.
    WorkStealingDeque deque(numTasks);

    std::atomic<int> completed{0};

    // Track whether every task runs exactly once.
    std::vector<std::atomic<int>> executions(numTasks);

    for (auto& count : executions) {
        count.store(0);
    }

    // -------------------------------------------------
    // Fill the deque before starting concurrent access.
    // -------------------------------------------------

    for (int i = 0; i < numTasks; ++i) {
        deque.push([i, &executions, &completed] {
            executions[i].fetch_add(
                1,
                std::memory_order_relaxed
            );

            completed.fetch_add(
                1,
                std::memory_order_relaxed
            );
        });
    }

    std::cout
        << "Finished pushing "
        << numTasks
        << " tasks\n";

    // -------------------------------------------------
    // Owner thread:
    // removes tasks from the bottom.
    // -------------------------------------------------

    std::thread owner([&] {
        WorkStealingDeque::Task task;

        while (completed.load(
                   std::memory_order_relaxed
               ) < numTasks) {

            if (deque.tryPop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    });

    // -------------------------------------------------
    // Thief threads:
    // steal tasks from the top.
    // -------------------------------------------------

    std::vector<std::thread> thieves;

    for (int i = 0; i < numThieves; ++i) {

        thieves.emplace_back([&] {
            WorkStealingDeque::Task task;

            while (completed.load(
                       std::memory_order_relaxed
                   ) < numTasks) {

                if (deque.trySteal(task)) {
                    task();
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }

    owner.join();

    for (auto& thief : thieves) {
        thief.join();
    }

    // -------------------------------------------------
    // Verify every task ran exactly once.
    // -------------------------------------------------

    int missing = 0;
    int duplicates = 0;

    for (int i = 0; i < numTasks; ++i) {

        int count =
            executions[i].load(
                std::memory_order_relaxed
            );

        if (count == 0) {
            ++missing;
        }
        else if (count > 1) {
            ++duplicates;
        }
    }

    std::cout
        << "Expected: "
        << numTasks
        << '\n';

    std::cout
        << "Completed: "
        << completed.load()
        << '\n';

    std::cout
        << "Missing: "
        << missing
        << '\n';

    std::cout
        << "Duplicates: "
        << duplicates
        << '\n';

    if (completed.load() == numTasks &&
        missing == 0 &&
        duplicates == 0 &&
        deque.empty()) {

        std::cout
            << "Work-stealing deque test PASSED\n";
    }
    else {
        std::cout
            << "Work-stealing deque test FAILED\n";
    }

    return 0;
}