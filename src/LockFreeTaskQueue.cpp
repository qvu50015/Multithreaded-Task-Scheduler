#include "LockFreeTaskQueue.h"

LockFreeTaskQueue::LockFreeTaskQueue()
    : head(nullptr),
      retiredHead(nullptr)
{
}

LockFreeTaskQueue::~LockFreeTaskQueue()
{
    Node* current = head.load();

    while (current != nullptr) {
        Node* next = current->next.load();

        delete current;

        current = next;
    }

    current = retiredHead.load();

    while (current != nullptr) {
        Node* next = current->next.load();

        delete current;

        current = next;
    }
}

void LockFreeTaskQueue::push(Task task)
{
    Node* newNode = new Node(std::move(task));

    Node* oldHead =
        head.load(std::memory_order_relaxed);

    do {
        newNode->next.store(
            oldHead,
            std::memory_order_relaxed
        );
    }
    while (!head.compare_exchange_weak(
        oldHead,
        newNode,
        std::memory_order_release,
        std::memory_order_relaxed
    ));
}

bool LockFreeTaskQueue::tryPop(Task& task)
{
    Node* oldHead =
        head.load(std::memory_order_acquire);

    while (oldHead != nullptr) {

        Node* next =
            oldHead->next.load(
                std::memory_order_relaxed
            );

        if (head.compare_exchange_weak(
                oldHead,
                next,
                std::memory_order_acquire,
                std::memory_order_relaxed
            )) {

            task = std::move(oldHead->task);

            retireNode(oldHead);

            return true;
        }
    }

    return false;
}

void LockFreeTaskQueue::retireNode(Node* node)
{
    Node* oldRetired =
        retiredHead.load(
            std::memory_order_relaxed
        );

    do {
        node->next.store(
            oldRetired,
            std::memory_order_relaxed
        );
    }
    while (!retiredHead.compare_exchange_weak(
        oldRetired,
        node,
        std::memory_order_release,
        std::memory_order_relaxed
    ));
}