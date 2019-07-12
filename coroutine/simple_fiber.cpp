//          Copyright Oliver Kowalke 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <boost/fiber/all.hpp>

inline void fn( std::string const& str, int n) {
	for ( int i = 0; i < n; ++i) {
		std::cout << i << ": " << str << std::endl;
		boost::this_fiber::yield();
	}
}

int main() {
    try {
        boost::fibers::fiber f1( fn, "abc", 5);
        std::cerr << "f1 : " << f1.get_id() << std::endl;
        f1.join();
        std::cout << "done." << std::endl;

        return EXIT_SUCCESS;
    } catch ( std::exception const& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "unhandled exception" << std::endl;
    }
	return EXIT_FAILURE;
}

/**
 * https://www.boost.org/doc/libs/1_70_0/libs/fiber/examples/simple.cpp
 * https://www.boost.org/doc/libs/1_70_0/libs/fiber/doc/html/
 * MacBook-Pro:coroutine qingdujun$ ./simple 
 * f1 : 0x108d41f00
 * 0: abc
 * 1: abc
 * 2: abc
 * 3: abc
 * 4: abc
 * done.
 */