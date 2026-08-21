#include <iostream>
#include <thread>

void worker1(){
    std::cout << "Worker 1 started" << std::endl;
}

void worker2(){
    std::cout << "Worker 2 started" << std::endl;
}

void worker3(){
    std::cout << "Worker 3 started " << std::endl;
}

void worker4(){
    std::cout << "Worker 4 started" << std::endl;
}


int main()
{
    std::thread t1(worker1);
    std::thread t2(worker2);
    std::thread t3(worker3);
    std::thread t4(worker4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}

