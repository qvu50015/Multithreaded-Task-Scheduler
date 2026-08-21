#include <iostream>
#include <thread>

void worker(int id){
    std::cout << "Worker "<< id << " started" << std::endl;

    for(int i = 0; i < 1000000; i++){

    }
}

int main()
{
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    std::thread t3(worker, 3);
    std::thread t4(worker, 4);

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

