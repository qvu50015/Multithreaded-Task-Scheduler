#include "ThreadPool.h"
#include <iostream>

int main(){
    ThreadPool pool(10);
    
    for (int i = 0; i < 100; i++) {
    pool.enqueue([i] {
        std::cout << "Task " << i << " executing\n";
    });
}
    
    return 0;
}

