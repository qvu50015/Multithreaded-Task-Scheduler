#include "ThreadPool.h"
#include <iostream>

int main()
{
    ThreadPool pool(4);

    auto future1 = pool.enqueue([] {
        return 42;
    });

    auto future2 = pool.enqueue([] {
        return 3.14;
    });

    std::cout << future1.get() << '\n';
    std::cout << future2.get() << '\n';

    return 0;
}

