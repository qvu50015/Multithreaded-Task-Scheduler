#include "ThreadPool.h"
#include <atomic>
#include <cassert>
#include <iostream>

void runTest(int numTasks){
    std::atomic<int> counter{0};
    {
        ThreadPool pool(4);

        for (int i = 0; i < numTasks; ++i) {
            pool.enqueue([&counter] {
                counter.fetch_add(1, std::memory_order_relaxed);
            });
        }
    }

    assert(counter.load() == numTasks);

    std::cout << "Test Counter ["
              << numTasks
              << "]: "
              << counter.load()
              << " PASSED\n";
}

int main(){
    runTest(100);
    runTest(1000);
    runTest(10000);
    runTest(100000);

    return 0;
}
