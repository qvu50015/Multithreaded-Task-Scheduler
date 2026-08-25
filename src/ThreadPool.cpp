#include "ThreadPool.h"
#include <iostream>

thread_local int currentWorkerId = 0;

int getCurrentWorkerId()
{
    return currentWorkerId;
}

ThreadPool::ThreadPool(size_t numWorkers)
{
    for (size_t i = 0; i < numWorkers; i++) {
        workerQueues.push_back(
            std::make_unique<TaskQueue>()
        );
    }

    for (size_t i = 0; i < numWorkers; i++) {
        workers.emplace_back(
            &ThreadPool::worker,
            this,
            i + 1
        );
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

void ThreadPool::worker(int id)
{
    currentWorkerId = id;
    std::cout << "Worker " << id << " started\n";

    size_t workerIndex = id - 1;

    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(conditionMutex);

            taskCondition.wait(lock, [this, workerIndex] {
                return !workerQueues[workerIndex]->empty() || !running;
            });

            if (workerQueues[workerIndex]->empty() && !running) {
                return;
            }

            workerQueues[workerIndex]->tryPop(task);
        }

        if (task) {
            task();
        }
    }
}