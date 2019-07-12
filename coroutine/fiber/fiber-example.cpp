#include <iostream>
#include <string>
#include <boost/fiber/all.hpp>

void foo(boost::fibers::buffered_channel<std::string>& channel) {
    channel.push("hello");
    channel.push(",");
    channel.push("Web");
    channel.push("Surfing");
    channel.push("!");
    channel.close();
}

void bar(boost::fibers::buffered_channel<std::string>& channel) {
    for (std::string& s : channel) {
        std::cout << s << " ";
    }
    channel.close();
} 

int main(int argc, char const *argv[]) {
    boost::fibers::buffered_channel<std::string> channel(4);
    boost::fibers::fiber fb1(foo, std::ref(channel));
    boost::fibers::fiber fb2(std::bind(bar, std::ref(channel)));

    fb1.join();
    fb2.join();
    std::cout << std::endl;

    return 0;
}
