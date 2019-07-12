#include <iostream>
#include <boost/coroutine2/all.hpp>

void foo(boost::coroutines2::coroutine<int>::pull_type& pull) {
    std::cout << "a";
    std::cout << pull.get();
    std::cout << "b";
    pull();
    std::cout << "c";
    for (auto val : pull) {
        std::cout << val;
    }
}

int main(int argc, char const *argv[]) {
    boost::coroutines2::coroutine<int>::push_type push(foo);    

    std::cout << "x";
    push(1);
    std::cout << "y";
    push(2);
    std::cout << "z";
    push(3);
    std::cout << "i";
    push(4);
    std::cout << "j" << std::endl;

    return 0;
}
/**
 * MacBook-Pro:coroutine qingdujun$ ./example 
 * x
 * a
 * 1
 * b
 * y
 * c
 * 2
 * z
 * 3
 * i
 * 4
 * j
 */