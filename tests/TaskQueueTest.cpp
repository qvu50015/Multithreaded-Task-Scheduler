#include "TaskQueue.h"
#include <iostream>

int main()
{
    TaskQueue queue;

    queue.push([] {
        std::cout << "Task 1 executing\n";
    });

    queue.push([] {
        std::cout << "Task 2 executing\n";
    });

    std::function<void()> task;

    while (queue.tryPop(task)) {
        task();
    }

    std::cout << "Queue empty: "
              << std::boolalpha
              << queue.empty()
              << '\n';

    return 0;
}