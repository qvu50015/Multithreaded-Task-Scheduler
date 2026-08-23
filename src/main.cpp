#include "ThreadPool.h"
#include <iostream>

int main(){
    ThreadPool pool(4);
    pool.enqueue([] {
        std::cout << "Task 1 executing\n";
    });

    pool.enqueue([] {
        std::cout << "Task 2 executing\n";
    });
    
    return 0;
}

