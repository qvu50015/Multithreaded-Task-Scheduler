#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <queue>
#include <mutex>
#include "PriorityTask.h"

class PriorityQueue {
private:
    std::priority_queue<PriorityTask> tasks;
    mutable std::mutex mutex;

public:
    void push(PriorityTask task);
    bool tryPop(PriorityTask& task);
    bool empty() const;
};

#endif