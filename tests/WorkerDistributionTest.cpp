#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <future>

int main(){
    ThreadPool pool(4);

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 12; i++) {
        futures.push_back(
            pool.enqueue([i] {
                std::cout << "Task "
                          << i
                          << " executed by Worker "
                          << getCurrentWorkerId()
                          << '\n';
            })
        );
    }

    for (auto& future : futures){
        future.get();
    }

    return 0;
}
