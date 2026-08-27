#include "ThreadPool.h"
#include <iostream>

thread_local int currentWorkerId = 0;

int getCurrentWorkerId()
{
    return currentWorkerId;
}

ThreadPool::ThreadPool(size_t numWorkers)
{
    for (size_t i = 0; i < numWorkers; i++)
    {
        workerQueues.push_back(
            std::make_unique<TaskQueue>());
    }

    for (size_t i = 0; i < numWorkers; i++)
    {
        workers.emplace_back(
            &ThreadPool::worker,
            this,
            i + 1);
    }
}

ThreadPool::~ThreadPool()
{
    running = false;
    taskCondition.notify_all();

    for (auto &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::worker(int id)
{
    currentWorkerId = id;
    std::cout << "Worker " << id << " started\n";

    size_t workerIndex = id - 1;

    while (true)
    {
        std::function<void()> task;

        // 1. Try our own queue
        if (workerQueues[workerIndex]->tryPop(task))
        {
            task();
            continue;
        }

        // 2. Try to steal from another worker
        for (size_t i = 0; i < workerQueues.size(); i++)
        {

            if (i == workerIndex)
            {
                continue;
            }

            if (workerQueues[i]->trySteal(task))
            {
                std::cout << "Worker " << id << " stole a task from Worker " << i + 1 << '\n';
                task();
                break;
            }
        }

        // If we stole a task, execute it and continue
        if (task)
        {
            continue;
        }

        // 3. No task found anywhere
        std::unique_lock<std::mutex> lock(conditionMutex);

        taskCondition.wait(lock, [this]
                           {
    if (!running) {
        return true;
    }

    for (const auto& queue : workerQueues) {
        if (!queue->empty()) {
            return true;
        }
    }

    return false; });

        // 4. If shutting down and no local work remains, exit
        if (!running && workerQueues[workerIndex]->empty())
        {
            return;
        }
    }
}