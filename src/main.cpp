#include <iostream>
#include <thread>
#include <functional>
#include <queue> 
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

std::condition_variable taskCondition;
std::queue<std::function<void()>> tasks;
std::mutex taskMutex;
std::atomic<bool> running = true;


void worker(int id){
    std::cout << "Worker "<< id << " started" << std::endl;

    while(running)
    {
        std::function<void()> task;{
            std::unique_lock<std::mutex> lock(taskMutex);
           

            taskCondition.wait(lock, [] {
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

int main(){
    tasks.push([] {std::cout << "Task 1 executing\n";});
    tasks.push([] {std::cout << "Task 2 executing\n";});

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    std::thread t3(worker, 3);
    std::thread t4(worker, 4);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    running = false;
    taskCondition.notify_all();

    if(t1.joinable()){
        t1.join();
        std::cout << "Worker 1 finished" << std::endl;
    }

    if(t2.joinable()){
        t2.join();
        std::cout << "Worker 2 finished" << std::endl;
    }

    if(t3.joinable()){
        t3.join();
        std::cout << "Worker 3 finished" << std::endl;
    }

    if(t4.joinable()){
        t4.join();
        std::cout << "Worker 4 finished" << std::endl;
    }


    return 0;
}

