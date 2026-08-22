#include <iostream>
#include <thread>
#include <functional>

void worker(int id, std::function<void()> task){
    std::cout << "Worker "<< id << " started" << std::endl;
    task();

}

int main()
{
    std::thread t1(worker, 1, []{std::cout << "Task 1 executing\n";});
    std::thread t2(worker, 2, []{std::cout << "Task 2 executing\n";});
    std::thread t3(worker, 3, []{std::cout << "Task 3 executing\n";});
    std::thread t4(worker, 4, []{std::cout << "Task 4 executing\n";});

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

