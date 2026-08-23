#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

class ThreadPool {
private:
    std::condition_variable taskCondition;
    std::queue<std::function<void()>> tasks;
    std::mutex taskMutex;
    std::atomic<bool> running = true;
    std::vector<std::thread> workers;

public:
    ThreadPool(size_t numWorkers);
    void enqueue(std::function<void()> task);
    ~ThreadPool();

private:
    void worker(int id);
};

#endif