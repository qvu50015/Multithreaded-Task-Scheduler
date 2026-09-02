#include "WorkStealingDeque.h"

#include <stdexcept>
#include <utility>

WorkStealingDeque::WorkStealingDeque(
    std::size_t capacity
)
    : buffer(capacity),
      capacity(capacity)
{
    if (capacity == 0) {
        throw std::invalid_argument(
            "WorkStealingDeque capacity must be greater than 0"
        );
    }
}

void WorkStealingDeque::push(Task task)
{
    std::size_t b =
        bottom.load(std::memory_order_relaxed);

    std::size_t t =
        top.load(std::memory_order_acquire);

    if (b - t >= capacity) {
        throw std::runtime_error(
            "WorkStealingDeque is full"
        );
    }

    TaskPtr taskPtr =
        std::make_shared<Task>(std::move(task));

    std::atomic_store_explicit(
        &buffer[b % capacity],
        std::move(taskPtr),
        std::memory_order_relaxed
    );

    std::atomic_thread_fence(
        std::memory_order_release
    );

    bottom.store(
        b + 1,
        std::memory_order_relaxed
    );
}

bool WorkStealingDeque::trySteal(Task& task)
{
    std::size_t t =
        top.load(std::memory_order_acquire);

    std::atomic_thread_fence(
        std::memory_order_seq_cst
    );

    std::size_t b =
        bottom.load(std::memory_order_acquire);

    if (t >= b) {
        return false;
    }

    TaskPtr taskPtr =
        std::atomic_load_explicit(
            &buffer[t % capacity],
            std::memory_order_acquire
        );

    if (!taskPtr) {
        return false;
    }

    if (!top.compare_exchange_strong(
            t,
            t + 1,
            std::memory_order_seq_cst,
            std::memory_order_relaxed
        )) {
        return false;
    }

    task = *taskPtr;

    std::atomic_store_explicit(
        &buffer[t % capacity],
        TaskPtr{},
        std::memory_order_release
    );

    return true;
}

bool WorkStealingDeque::tryPop(Task& task)
{
    std::size_t b =
        bottom.load(std::memory_order_relaxed);

    if (b == 0) {
        return false;
    }

    b -= 1;

    bottom.store(
        b,
        std::memory_order_relaxed
    );

    std::atomic_thread_fence(
        std::memory_order_seq_cst
    );

    std::size_t t =
        top.load(std::memory_order_relaxed);

    if (t <= b) {

        TaskPtr taskPtr =
            std::atomic_load_explicit(
                &buffer[b % capacity],
                std::memory_order_acquire
            );

        if (!taskPtr) {
            bottom.store(
                b + 1,
                std::memory_order_relaxed
            );

            return false;
        }

        // Last task in the deque.
        if (t == b) {

            if (!top.compare_exchange_strong(
                    t,
                    t + 1,
                    std::memory_order_seq_cst,
                    std::memory_order_relaxed
                )) {

                // A thief won the race.
                bottom.store(
                    b + 1,
                    std::memory_order_relaxed
                );

                return false;
            }

            bottom.store(
                b + 1,
                std::memory_order_relaxed
            );
        }

        task = *taskPtr;

        std::atomic_store_explicit(
            &buffer[b % capacity],
            TaskPtr{},
            std::memory_order_release
        );

        return true;
    }

    bottom.store(
        b + 1,
        std::memory_order_relaxed
    );

    return false;
}

bool WorkStealingDeque::empty() const
{
    std::size_t t =
        top.load(std::memory_order_acquire);

    std::size_t b =
        bottom.load(std::memory_order_acquire);

    return t >= b;
}