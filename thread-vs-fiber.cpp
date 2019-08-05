#include <iostream>
#include <string>
#include <ctime>
#include <climits>
#include <thread>
#include <boost/fiber/all.hpp>

void hello_fiber() {
    for(int i = 0; i < 1000; ++i) {
	    boost::this_fiber::yield();
    }
}

void hello_thread() {
    for(int i = 0; i < 1000; ++i) {
	    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main() {
    clock_t start = 0, end = 0;
    start=clock();
    // boost::fibers::fiber f1(hello_fiber);
    // boost::fibers::fiber f2(hello_fiber);
    // boost::fibers::fiber f3(hello_fiber);
    // boost::fibers::fiber f4(hello_fiber);
    std::thread t1(hello_thread);
    std::thread t2(hello_thread);
    std::thread t3(hello_thread);
    std::thread t4(hello_thread);
    end=clock();

    std::cout << end - start << std::endl;
    start=clock();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    // f1.join();
    // f2.join();
    // f3.join();
    // f4.join();
    end=clock();

    std::cout << end - start << std::endl;
    std::cout << "done." << std::endl;
    return 0;
}

/**
 * 创建一个协程大约20-30毫秒
 * 协程切换大约400/4000=0.1毫秒
 * 
 * 创建一个线程大约114/4=(30-40)毫秒
 * 线程切换大约28毫秒（有误）
 */