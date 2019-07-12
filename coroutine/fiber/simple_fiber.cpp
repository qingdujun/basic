
#include <iostream>
#include <string>
#include <boost/fiber/all.hpp>

void fn(std::string const& str, int n) {
	for ( int i = 0; i < n; ++i) {
		std::cout << str << ": " << i << std::endl;
		boost::this_fiber::yield();
	}
}

int main() {
    boost::fibers::fiber f1(fn, "abc", 15);
    boost::fibers::fiber f2(fn, "efg", 2);
    boost::fibers::fiber f3(fn, "hij", 8);
    boost::fibers::fiber f4(fn, "klm", 10);

    // std::cerr << "f1 : " << f1.get_id() << std::endl;
    f1.join();
    f2.join();
    f3.join();
    f4.join();
    std::cout << "done." << std::endl;
    return 0;
}