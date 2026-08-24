#include "ThreadPool.h"
#include <iostream>
#include <atomic>

int main(){
    std::atomic<int> counter_100 = 0;
    std::atomic<int> counter_1000 = 0;
    std::atomic<int> counter_10000 = 0;
    std::atomic<int> counter_100000 = 0;

    {
        ThreadPool pool(4);
        for(int i = 0; i < 100; i++){
            pool.enqueue([&counter_100]{
                counter_100++;
            });
        }
    }

    {
        ThreadPool pool(4);

        auto submitTasks = [&pool, &counter_1000](){
            for (int i = 0; i < 250; i++) {
                pool.enqueue([&counter_1000] {
                    counter_1000++;
                });
            }
        };

        std::thread submitter1(submitTasks);
        std::thread submitter2(submitTasks);
        std::thread submitter3(submitTasks);
        std::thread submitter4(submitTasks);

        submitter1.join();
        submitter2.join();
        submitter3.join();
        submitter4.join();
    }

    {
        ThreadPool pool(4);

        auto submitTasks = [&counter_10000, &pool](){
            for(int i = 0; i < 2500; i++){
                pool.enqueue([&counter_10000]{
                    counter_10000++;
                });
            }
        };

        std::thread submitter1(submitTasks);
        std::thread submitter2(submitTasks);
        std::thread submitter3(submitTasks);
        std::thread submitter4(submitTasks);

        submitter1.join();
        submitter2.join();
        submitter3.join();
        submitter4.join();
    }

    {
        ThreadPool pool(4);

        auto submitTasks = [&counter_100000, &pool](){
            for(int i = 0; i < 25000; i++){
                pool.enqueue([&counter_100000]{
                    counter_100000++;
                });
            }
        };

        std::thread submitter1(submitTasks);
        std::thread submitter2(submitTasks);
        std::thread submitter3(submitTasks);
        std::thread submitter4(submitTasks);

        submitter1.join();
        submitter2.join();
        submitter3.join();
        submitter4.join();
    }

    std::cout << "Test Counter [100]: " << counter_100 << std::endl;
    std::cout << "Test Counter [1000]: " << counter_1000 << std::endl;
    std::cout << "Test Counter [10000]: " << counter_10000 << std::endl;
    std::cout << "Test Counter [100000]: " << counter_100000 << std::endl;

    return 0;
}