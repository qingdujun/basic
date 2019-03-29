#include <iostream>
#include "thread_pool.hpp"

void test_thread_pool() {
    ThreadPool pool;
    std::thread thd1([&pool] {
        for (int i = 0; i < 10; ++i) {
            auto thd_id = std::this_thread::get_id();//Returns the id of the current thread.
            pool.AddTask([thd_id] {
                std::cout << "同步层线程1的线程ID: " << thd_id << std::endl;
            });
        }
    });

    std::thread thd2([&pool] {
        for (int i = 0; i < 10; ++i) {
            auto thd_id = std::this_thread::get_id();
            pool.AddTask([thd_id] {
                std::cout << "同步层线程2的线程ID: " << thd_id << std::endl;
            });
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
    getchar();
    pool.Stop();
    thd1.join();
    thd2.join();
}

int main(void) {
    test_thread_pool();
    return 0;
}