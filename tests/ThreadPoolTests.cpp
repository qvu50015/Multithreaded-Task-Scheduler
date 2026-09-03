#include "ThreadPool.h"
#include <iostream>
#include <atomic>
#include <thread>

void runTest(int numTasks){
    std::atomic<int> counter = 0;
    {
        ThreadPool pool(4);

        for (int i = 0; i < numTasks; i++){
            pool.enqueue([&counter] {
                counter++;
            });
        }
    }

    std::cout << "Test Counter ["
              << numTasks
              << "]: "
              << counter
              << '\n';
}

int main(){
    runTest(100);
    runTest(1000);
    runTest(10000);
    runTest(100000);

    return 0;
}
