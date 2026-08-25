#ifndef PRIORITYTASK_H
#define PRIORITYTASK_H

#include <functional>
#include "Priority.h"

struct PriorityTask {
    Priority priority;
    std::function<void()> task;

    bool operator<(const PriorityTask& other) const {
        return priority > other.priority;
    }
};

#endif