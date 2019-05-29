/**
 * 
 * 2019-05-25 by Manlou Hua(hua7th@gmail.com)
 * 
 * **/

#include <iostream>
#include <cstdlib>
#include "Server.hpp"
using namespace ml;

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "$./server port" << std::endl;
    }
    boost::asio::io_service ios;
    // boost::asio::io_service::work work(ios);
    // std::thread thd([&ios] { ios.run(); });
    Server server(ios, static_cast<short>(std::atoi(argv[1])));
    server.Accept();
    ios.run();
    // thd.join();

    return 0;
}
