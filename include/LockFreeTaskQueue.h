#ifndef LOCKFREETASKQUEUE_H
#define LOCKFREETASKQUEUE_H

#include <atomic>
#include <functional>
#include <memory>
#include <utility>

class LockFreeTaskQueue{
public:
    using Task = std::function<void()>;

    LockFreeTaskQueue();
    ~LockFreeTaskQueue();

    LockFreeTaskQueue(const LockFreeTaskQueue&) = delete;
    LockFreeTaskQueue& operator=(const LockFreeTaskQueue&) = delete;

    void push(Task task);

    bool tryPop(Task& task);

private:
    struct Node
    {
        Task task;
        std::atomic<Node*> next;

        explicit Node(Task task)
            : task(std::move(task)),
              next(nullptr)
        {
        }
    };

    std::atomic<Node*> head;
    std::atomic<Node*> retiredHead;

    void retireNode(Node* node);
};

#endif
