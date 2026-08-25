#include "ThreadPool.h"
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    ThreadPool pool(4);

    auto future1 = pool.enqueue(Priority::HIGH, [] {
        return 42;
    });

    auto future2 = pool.enqueue(Priority::MEDIUM, [] {
        return 3.14;
    });

    auto future3 = pool.enqueue(Priority::LOW, [] {
        return std::string("Hello");
    });

    auto future4 = pool.enqueue(Priority::HIGH, [] {
        throw std::runtime_error("Task failed!");
        return 42;
    });

    try {
        std::cout << future4.get() << '\n';
    }
    catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    std::cout << future1.get() << '\n';
    std::cout << future2.get() << '\n';
    std::cout << future3.get() << '\n';

    return 0;
}