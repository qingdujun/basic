/**
 * 
 * 2019-05-25 by Manlou Hua(hua7th@gmail.com)
 * 
 * **/

#ifndef HTTP_PROXY_TEST_CLIENT_HPP
#define HTTP_PROXY_TEST_CLIENT_HPP

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <boost/thread.hpp>
#include "../src/RWHandler.hpp"

namespace ml {
const int MAX_RECONN_TIMES = 3;

class Connector {
public:
    Connector(boost::asio::io_service& ios, std::string const& str_ip, short port);
    ~Connector();
    bool Start();
    bool IsConnected() const;
    void Send(char* data, int length);
private:
    void CreateEventHandler(boost::asio::io_service& ios);
    void CheckConnect();
    void HandleConnectError(boost::system::error_code const& error);
    void HandleRWError(int conn_id);
private:
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::endpoint server_addr_;
    std::shared_ptr<RWHandler> event_handler_;
    bool is_connected_;
    std::shared_ptr<std::thread> chk_thread_;
    int reconn_times_;
};

Connector::Connector(boost::asio::io_service& ios, std::string const& str_ip, short port)
        : ios_(ios), socket_(ios), is_connected_(false), chk_thread_(nullptr), reconn_times_(0),
        server_addr_(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(str_ip), port)) {
            CreateEventHandler(ios);
        }

Connector::~Connector() {
    reconn_times_ = MAX_RECONN_TIMES+1;
    chk_thread_->join();
    // chk_thread_->detach();
}

bool Connector::Start() {
    event_handler_->GetSocket().async_connect(server_addr_, [this] (boost::system::error_code const& error) {
        if (error) {
            HandleConnectError(error);
            return;
        }
        std::cout << "connect ok" << std::endl;
        is_connected_ = true;
        reconn_times_ = 0;
        event_handler_->HandleRead();
    });
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    return is_connected_;
}

bool Connector::IsConnected() const {
    return is_connected_;
}

void Connector::Send(char* data, int length) {
    if (!IsConnected()) 
        return;
    event_handler_->HandleWrite(data, length);
}

void Connector::CreateEventHandler(boost::asio::io_service& ios) {
    event_handler_ = std::make_shared<RWHandler>(ios);
    event_handler_->SetCallbackError([this] (int conn_id) { HandleRWError(conn_id); });
}

void Connector::CheckConnect() {
    if (chk_thread_ != nullptr)
        return;
    chk_thread_ = std::make_shared<std::thread>([this] {
        while (reconn_times_ <= MAX_RECONN_TIMES) {
            if (!IsConnected()) {
                std::cout << "trying to reconnect " << ++reconn_times_ <<" times"<< std::endl;
                Start();
            }
            boost::this_thread::sleep(boost::posix_time::seconds(1));
        }
        std::cout << "@^@ : reconnect failed" << std::endl;
    });
}

void Connector::HandleConnectError(boost::system::error_code const& error) {
    is_connected_ = false;
    std::cout << error.message() << std::endl;
    event_handler_->CloseSocket();
    CheckConnect();
}

void Connector::HandleRWError(int conn_id) {
    is_connected_ = false;
    CheckConnect();
}

}


#endif