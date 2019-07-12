#include <iostream>
#include <boost/coroutine2/all.hpp>

void foo(boost::coroutines2::coroutine<void>::pull_type& pull) {
    std::cout << "a" << std::endl;
    pull();
    std::cout << "b" << std::endl;
    pull();
    std::cout << "c" << std::endl;
}

int main(int argc, char const *argv[]) {
    boost::coroutines2::coroutine<void>::push_type push(foo);    

    std::cout << "1" << std::endl;
    push();
    std::cout << "2" << std::endl;
    push();
    std::cout << "3" << std::endl;
    
    return 0;
}
