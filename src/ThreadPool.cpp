#include "ThreadPool.h"
#include <iostream>

thread_local int currentWorkerId = 0;

int getCurrentWorkerId()
{
    return currentWorkerId;
}

ThreadPool::ThreadPool(size_t numWorkers)
{
    for (size_t i = 0; i < numWorkers; i++){
        workers.emplace_back(&ThreadPool::worker, this, i + 1);
    }
}

ThreadPool::~ThreadPool(){
    running = false;
    taskCondition.notify_all();

    for (auto& worker : workers){
        if (worker.joinable()){
            worker.join();
        }
    }
}

void ThreadPool::worker(int id){
    currentWorkerId = id;

    std::cout << "Worker "<< id << " started" << std::endl;

    while(true)
    {
        std::function<void()> task;{
            std::unique_lock<std::mutex> lock(conditionMutex);

            taskCondition.wait(lock, [this] {
                return !tasks.empty() || !running;
            });

            if(tasks.empty() && !running){
                return;
            }

            tasks.tryPop(task);
        }

        if(task){
            task();
        }
    }
}