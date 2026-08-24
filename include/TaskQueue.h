#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <functional>
#include <queue>
#include <mutex>

class TaskQueue {
private:
    std::queue<std::function<void()>> tasks;
    mutable std::mutex mutex;

public:
    void push(std::function<void()> task);

    bool tryPop(std::function<void()>& task);

    bool empty() const;
};

#endif