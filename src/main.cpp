#include "ThreadPool.h"
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    ThreadPool pool(4);

    auto intFuture = pool.enqueue([] {
        return 42;
    });

    auto doubleFuture = pool.enqueue([] {
        return 3.14;
    });

    auto stringFuture = pool.enqueue([] {
        return std::string("Hello from the ThreadPool");
    });

    auto exceptionFuture = pool.enqueue([]() -> int {
        throw std::runtime_error("Task failed! (Expected)");
    });

    std::cout << "Integer result: "
              << intFuture.get() << '\n';

    std::cout << "Double result: "
              << doubleFuture.get() << '\n';

    std::cout << "String result: "
              << stringFuture.get() << '\n';

    try {
        exceptionFuture.get();
    }
    catch (const std::exception& e) {
        std::cout << "Caught task exception: "
                  << e.what() << '\n';
    }

    return 0;
}