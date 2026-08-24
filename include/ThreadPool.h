#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <future>
#include <type_traits>
#include <memory>

int getCurrentWorkerId();

class ThreadPool {
private:
    std::condition_variable taskCondition;
    std::queue<std::function<void()>> tasks;
    std::mutex taskMutex;
    std::atomic<bool> running = true;
    std::vector<std::thread> workers;

public:
    ThreadPool(size_t numWorkers);

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
            std::move(packagedTask)
        );

    {
        std::lock_guard<std::mutex> lock(taskMutex);

        tasks.push([taskWrapper]() {
            (*taskWrapper)();
        });
    }

    taskCondition.notify_one();

    return future;
}

#endif