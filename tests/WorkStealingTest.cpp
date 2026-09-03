#include "ThreadPool.h"
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main(){
    ThreadPool pool(4);

    std::vector<std::future<void>> futures;
    std::mutex outputMutex;

    std::cout << "Work-Stealing Demo\n";
    std::cout << "4 workers, 20 tasks\n";
    std::cout << "Every 4th task takes 400 ms; all others take 50 ms.\n\n";

    std::cout << "Initial round-robin assignment:\n";
    std::cout << "Worker 1: [0 4 8 12 16]   <- all expensive tasks\n";
    std::cout << "Worker 2: [1 5 9 13 17]\n";
    std::cout << "Worker 3: [2 6 10 14 18]\n";
    std::cout << "Worker 4: [3 7 11 15 19]\n\n";

    std::cout << "Workers 2-4 should finish their short tasks first,\n";
    std::cout << "then steal expensive tasks from Worker 1.\n\n";

    for (int i = 0; i < 20; ++i){
        futures.push_back(
            pool.enqueue([i, &outputMutex]{
                int duration = (i % 4 == 0) ? 400 : 50;

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(duration)
                );

                std::lock_guard<std::mutex> lock(outputMutex);

                std::cout
                    << "Task " << i
                    << " | " << duration << " ms"
                    << " | Worker " << getCurrentWorkerId()
                    << '\n';
            })
        );
    }

    for (auto& future : futures){
        future.get();
    }

    std::cout << "\nWork-stealing demo completed.\n";
    std::cout << "Long tasks 0, 4, 8, 12, and 16 started in Worker 1's queue.\n";
    std::cout << "If any of those tasks were executed by Workers 2-4,\n";
    std::cout << "those workers successfully stole work from Worker 1.\n";

    return 0;
}
