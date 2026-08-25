#include "PriorityQueue.h"
#include <iostream>

int main()
{
    PriorityQueue queue;

    queue.push({
        Priority::LOW,
        [] {
            std::cout << "Low priority task\n";
        }
    });

    queue.push({
        Priority::HIGH,
        [] {
            std::cout << "High priority task\n";
        }
    });

    queue.push({
        Priority::MEDIUM,
        [] {
            std::cout << "Medium priority task\n";
        }
    });

    PriorityTask task;

    while (queue.tryPop(task)) {
        task.task();
    }

    return 0;
}