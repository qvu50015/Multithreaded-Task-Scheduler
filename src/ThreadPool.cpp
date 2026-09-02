#include "ThreadPool.h"
#include <iostream>

thread_local int currentWorkerId = 0;

int getCurrentWorkerId()
{
    return currentWorkerId;
}

ThreadPool::ThreadPool(
    size_t numWorkers,
    bool useLockFree
)
    : useLockFree(useLockFree)
{
    for (size_t i = 0; i < numWorkers; ++i) {

        if (useLockFree) {
            lockFreeQueues.push_back(
                std::make_unique<WorkStealingDeque>(
                    200000
                )
            );
        }
        else {
            workerQueues.push_back(
                std::make_unique<TaskQueue>()
            );
        }
    }

    for (size_t i = 0; i < numWorkers; ++i) {
        workers.emplace_back(
            &ThreadPool::worker,
            this,
            i + 1
        );
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
        bool foundTask = false;

        if (useLockFree)
        {
            foundTask =
                lockFreeQueues[workerIndex]->tryPop(task);
        }
        else
        {
            foundTask =
                workerQueues[workerIndex]->tryPop(task);
        }

        if (foundTask)
        {
            task();
            continue;
        }

        // 2. Try to steal from another worker
        size_t numQueues =
            useLockFree
                ? lockFreeQueues.size()
                : workerQueues.size();

        bool stoleTask = false;

        for (size_t i = 0; i < numQueues; ++i)
        {
            if (i == workerIndex)
            {
                continue;
            }

            if (useLockFree)
            {
                stoleTask =
                    lockFreeQueues[i]->trySteal(task);
            }
            else
            {
                stoleTask =
                    workerQueues[i]->trySteal(task);
            }

            if (stoleTask)
            {
                break;
            }
        }

        if (stoleTask)
        {
            task();
            continue;
        }

        // 3. No task found anywhere
        std::unique_lock<std::mutex> lock(conditionMutex);

        taskCondition.wait(lock, [this]
        {
            if (!running)
            {
                return true;
            }

            if (useLockFree)
            {
                for (const auto& queue : lockFreeQueues)
                {
                    if (!queue->empty())
                    {
                        return true;
                    }
                }
            }
            else
            {
                for (const auto& queue : workerQueues)
                {
                    if (!queue->empty())
                    {
                        return true;
                    }
                }
            }

            return false;
        });

        // 4. If shutting down and no local work remains, exit
        bool localEmpty;

        if (useLockFree)
        {
            localEmpty =
                lockFreeQueues[workerIndex]->empty();
        }
        else
        {
            localEmpty =
                workerQueues[workerIndex]->empty();
        }

        if (!running && localEmpty)
        {
            return;
        }
    }
}