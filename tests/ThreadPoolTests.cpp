#include "ThreadPool.h"
#include <iostream>
#include <atomic>

int main(){
    std::atomic<int> counter_100 = 0;

    {
        ThreadPool pool(4);
        for(int i = 0; i < 100; i++){
            pool.enqueue([&counter_100]{
                counter_100++;
            });
        }
    }


    std::cout << "Test Counter [100]: " << counter_100 << '\n';

    return 0;
}