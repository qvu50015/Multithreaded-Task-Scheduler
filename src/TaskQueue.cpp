#include "TaskQueue.h"

void TaskQueue::push(std::function<void()> task)
{
    std::lock_guard<std::mutex> lock(mutex);
    tasks.push_back(std::move(task));
}

bool TaskQueue::tryPop(std::function<void()>& task)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (tasks.empty()) {
        return false;
    }

    task = std::move(tasks.front());
    tasks.pop_front();

    return true;
}

bool TaskQueue::empty() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return tasks.empty();
}

bool TaskQueue::trySteal(std::function<void()>& task)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (tasks.empty()) {
        return false;
    }

    task = std::move(tasks.back());
    tasks.pop_back();

    return true;
}