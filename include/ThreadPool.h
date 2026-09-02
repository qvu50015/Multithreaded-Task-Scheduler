#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "TaskQueue.h"
#include "WorkStealingDeque.h"
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <future>
#include <type_traits>
#include <memory>

int getCurrentWorkerId();

class ThreadPool
{
private:
    std::condition_variable taskCondition;
    std::mutex conditionMutex;
    bool useLockFree;

    std::vector<std::unique_ptr<TaskQueue>> workerQueues;
    std::vector<std::unique_ptr<WorkStealingDeque>> lockFreeQueues;
    std::atomic<size_t> nextWorker = 0;
    std::atomic<bool> running = true;

    std::vector<std::thread> workers;

public:
    ThreadPool(size_t numWorkers, bool useLockFree = false);

    template <typename F>
    auto enqueue(F task);

    ~ThreadPool();

private:
    void worker(int id);
};

template <typename F>
auto ThreadPool::enqueue(F task)
{
    using ReturnType = std::invoke_result_t<F>;

    std::packaged_task<ReturnType()> packagedTask(task);

    auto future = packagedTask.get_future();

    auto taskWrapper =
        std::make_shared<std::packaged_task<ReturnType()>>(
            std::move(packagedTask));

    size_t numQueues =
        useLockFree
            ? lockFreeQueues.size()
            : workerQueues.size();

    size_t workerIndex =
        nextWorker.fetch_add(1) % numQueues;

    auto wrapper = [taskWrapper]()
    {
        (*taskWrapper)();
    };

    if (useLockFree)
    {
        lockFreeQueues[workerIndex]->push(
            std::move(wrapper));
    }
    else
    {
        workerQueues[workerIndex]->push(
            std::move(wrapper));
    }

    taskCondition.notify_all();

    return future;
}

#endif