#include "ThreadPool.h"
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <chrono>

int main(){
    ThreadPool pool(4);

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 20; i++){
        futures.push_back(
            pool.enqueue([i]
            {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(100)
                );

                std::cout << "Task " << i
                          << " executed by Worker "
                          << getCurrentWorkerId()
                          << '\n';
            })
        );
    }

    for (auto& future : futures){
        future.get();
    }

    std::cout << "Work stealing test completed.\n";

    return 0;
}
