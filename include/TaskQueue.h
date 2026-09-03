#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <functional>
#include <deque>
#include <mutex>

class TaskQueue {
private:
    std::deque<std::function<void()>> tasks;
    mutable std::mutex mutex;

public:
    void push(std::function<void()> task);

    bool tryPop(std::function<void()>& task);

    bool empty() const;

    bool trySteal(std::function<void()>& task);
};

#endif
