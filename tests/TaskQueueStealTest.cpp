#include "TaskQueue.h"
#include <iostream>

int main()
{
    TaskQueue queue;

    queue.push([] {
        std::cout << "Task 1\n";
    });

    queue.push([] {
        std::cout << "Task 2\n";
    });

    queue.push([] {
        std::cout << "Task 3\n";
    });

    std::function<void()> task;

    if (queue.trySteal(task)) {
        task();
    }

    return 0;
}