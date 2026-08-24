#include "ThreadPool.h"
#include <iostream>
#include <stdexcept>

int main()
{
    ThreadPool pool(4);

    auto future1 = pool.enqueue([] {
        return 42;
    });

    auto future2 = pool.enqueue([] {
        return 3.14;
    });

    auto future3 = pool.enqueue([] {
        return std::string("Hello");
    });

    auto future4 = pool.enqueue([] {
    throw std::runtime_error("Task failed!");
    return 42;});

    try{
        std::cout << future4.get() << '\n';
    }
    catch(const std::exception& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    std::cout << future1.get() << '\n';
    std::cout << future2.get() << '\n';
    std::cout << future3.get() << '\n';

    return 0;
}
