#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numWorkers)
{
    for (size_t i = 0; i < numWorkers; i++){
        workers.emplace_back(&ThreadPool::worker, this, i + 1);
    }
}

void ThreadPool::enqueue(std::function<void()> task){
    {
    std::lock_guard<std::mutex> lock(taskMutex);
    tasks.push(task);
    }
    taskCondition.notify_one();
}

ThreadPool::~ThreadPool(){
    running = false;
    taskCondition.notify_all();

    for (auto& worker : workers){
        if (worker.joinable()){
            worker.join();
        }
    }
}

void ThreadPool::worker(int id){
    std::cout << "Worker "<< id << " started" << std::endl;

    while(running)
    {
        std::function<void()> task;{
            std::unique_lock<std::mutex> lock(taskMutex);
           

            taskCondition.wait(lock, [this] {
                return !tasks.empty() || !running;
            });

            if(tasks.empty() && !running){
                return;
            }

            task = tasks.front();
            tasks.pop();
            

        }

        if(task){
        task();
        }
    }
}