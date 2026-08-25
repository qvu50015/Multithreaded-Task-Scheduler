#include "PriorityQueue.h"

void PriorityQueue::push(PriorityTask task)
{
    std::lock_guard<std::mutex> lock(mutex);
    tasks.push(std::move(task));
}

bool PriorityQueue::tryPop(PriorityTask& task)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (tasks.empty()) {
        return false;
    }

    task = std::move(const_cast<PriorityTask&>(tasks.top()));
    tasks.pop();

    return true;
}

bool PriorityQueue::empty() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return tasks.empty();
}