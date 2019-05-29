/**
 * 
 * 2019-05-25 by Manlou Hua(hua7th@gmail.com)
 * 
 * **/

#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Client.hpp"
using namespace ml;

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cout << "$./client port" << std::endl;
    }
    boost::asio::io_service ios;
    boost::asio::io_service::work work(ios);
    boost::thread thd([&ios] {ios.run(); });

    Connector conn(ios, "127.0.0.1", static_cast<short>(std::atoi(argv[1])));
    conn.Start();

    if (conn.IsConnected()) {
        const int length = 512;
        char line[length] = "";
        std::string str;
        while (std::cin >> str) {
            char header[HEAD_LEN] = {};
            int total_length = str.length() + 1 + HEAD_LEN;
            std::sprintf(header, "%d", total_length);
            memcpy(line, header, HEAD_LEN);
            memcpy(line + HEAD_LEN, str.c_str(), str.length() + 1);
            conn.Send(line, total_length);
        }
    } 

    return 0;
}
