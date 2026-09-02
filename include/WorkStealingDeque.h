#ifndef WORKSTEALINGDEQUE_H
#define WORKSTEALINGDEQUE_H

#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

class WorkStealingDeque
{
public:
    using Task = std::function<void()>;

    explicit WorkStealingDeque(
        std::size_t capacity = 1024
    );

    void push(Task task);

    bool tryPop(Task& task);

    bool trySteal(Task& task);

    bool empty() const;

private:
    using TaskPtr = std::shared_ptr<Task>;

    std::vector<TaskPtr> buffer;

    std::atomic<std::size_t> top{0};
    std::atomic<std::size_t> bottom{0};

    std::size_t capacity;
};

#endif