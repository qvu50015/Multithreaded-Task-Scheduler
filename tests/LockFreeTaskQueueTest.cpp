#include "LockFreeTaskQueue.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main(){
    LockFreeTaskQueue queue;

    constexpr int numThreads = 4;
    constexpr int tasksPerThread = 25000;
    constexpr int totalTasks = numThreads * tasksPerThread;

    std::vector<std::thread> producers;

    for (int t = 0; t < numThreads; ++t){
        producers.emplace_back([&queue] {

            for (int i = 0; i < tasksPerThread; ++i){

                queue.push([] {
                    // Empty task for testing
                });
            }
        });
    }

    for (auto& producer : producers){
        producer.join();
    }

    std::cout << "Finished pushing " << totalTasks << " tasks\n";

    std::atomic<int> popped{0};
    std::vector<std::thread> consumers;

    for (int t = 0; t < numThreads; ++t){
        consumers.emplace_back([&] {

            LockFreeTaskQueue::Task task;

            while (true){

                if (queue.tryPop(task)){

                    task();

                    int count = popped.fetch_add(1) + 1;

                    if (count >= totalTasks) {
                        break;
                    }

                } 
                
                else{

                    if (popped.load() >= totalTasks){
                        break;
                    }

                    std::this_thread::yield();
                }
            }
        });
    }

    for(auto& consumer : consumers){
        consumer.join();
    }

    std::cout << "Expected: " << totalTasks << '\n';

    std::cout << "Popped: " << popped.load() << '\n';

    if(popped.load() == totalTasks){
        std::cout << "Lock-free test PASSED\n";
    } 
    
    else{
        std::cout << "Lock-free test FAILED\n";
    }

    return 0;
}
