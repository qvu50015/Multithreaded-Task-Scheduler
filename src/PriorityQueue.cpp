#include "PriorityQueue.h"

void PriorityQueue::push(PriorityTask task)
{
    tasks.push(std::move(task));
}

bool PriorityQueue::tryPop(PriorityTask& task)
{
    if (tasks.empty()) {
        return false;
    }

    task = std::move(const_cast<PriorityTask&>(tasks.top()));
    tasks.pop();

    return true;
}

bool PriorityQueue::empty() const
{
    return tasks.empty();
}