#include "TaskQueue.h"

void TaskQueue::push(std::function<void()> task)
{
    std::lock_guard<std::mutex> lock(mutex);

    tasks.push(std::move(task));
}

bool TaskQueue::tryPop(std::function<void()>& task)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (tasks.empty()) {
        return false;
    }

    task = std::move(tasks.front());
    tasks.pop();

    return true;
}

bool TaskQueue::empty() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return tasks.empty();
}
