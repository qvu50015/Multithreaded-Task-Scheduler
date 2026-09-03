#include "ThreadPool.h"
#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

int main(){
    std::cout << "=== Multithreaded Task Scheduler Demo ===\n\n";

    ThreadPool pool(4);

    std::cout << "Submitting tasks to 4 worker threads...\n\n";

    auto intFuture = pool.enqueue([] {
        return 42;
    });

    auto stringFuture = pool.enqueue([] {
        return std::string("Hello from the ThreadPool");
    });

    std::vector<std::future<int>> taskFutures;

    for (int i = 1; i <= 8; ++i) {
        taskFutures.push_back(
            pool.enqueue([i] {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(100)
                );

                return i * i;
            })
        );
    }

    std::cout << "Integer result: " << intFuture.get() << '\n';

    std::cout << "String result: " << stringFuture.get() << "\n\n";

    std::cout << "Results from 8 scheduled tasks:\n";

    for (std::size_t i = 0; i < taskFutures.size(); ++i) {
        std::cout << "Task "
                  << i + 1
                  << " result: "
                  << taskFutures[i].get()
                  << '\n';
    }

    auto exceptionFuture = pool.enqueue([]() -> int {
        throw std::runtime_error("Task failed! (Expected)");
    });

    std::cout << "\nTesting exception propagation...\n";

    try {
        exceptionFuture.get();
    }
    
    catch (const std::exception& e) {
        std::cout << "Caught task exception: "
                  << e.what()
                  << '\n';
    }

    std::cout << "\nDemo complete.\n";

    return 0;
}
