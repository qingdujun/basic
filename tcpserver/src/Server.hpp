/**
 * 
 * 2019-05-25 by Manlou Hua(hua7th@gmail.com)
 * 
 * **/

#ifndef HTTP_PROXY_SRC_SERVER_HPP
#define HTTP_PROXY_SRC_SERVER_HPP

#include <boost/asio/buffer.hpp>
#include <boost/asio.hpp>
#include <unordered_map>
#include <numeric>
#include <list>
#include <memory>
#include "RWHandler.hpp"
using namespace boost;

namespace ml {

const int MAX_CONN_NUM = 65536;
const int MAX_RECV_SIZE = 65536;

class Server {
public:
    Server(boost::asio::io_service& ios, short port);
    ~Server();
    void Accept();

private:
    void HandlerAcceptError(std::shared_ptr<RWHandler> eventHandler, boost::system::error_code const& error);
    void StopAccept();
    std::shared_ptr<RWHandler> CreateHandler();
    void RecyclConnId(int conn_id);
private:
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map<int, std::shared_ptr<RWHandler>> handlers_;
    std::list<int> conn_id_pool_;
};

Server::Server(boost::asio::io_service& ios, short port) : ios_(ios), conn_id_pool_(MAX_CONN_NUM),
    acceptor_(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
        conn_id_pool_.resize(MAX_CONN_NUM);
        std::iota(conn_id_pool_.begin(), conn_id_pool_.end(), 1);
}

Server::~Server() {
}

void Server::Accept() {
    std::cout << "Start Listening "<< std::endl;
    std::shared_ptr<RWHandler> handler = CreateHandler();
    acceptor_.async_accept(handler->GetSocket(), [this, handler] (boost::system::error_code const& error) {
        if (error) {
            std::cout << error.value() << " " << error.message() << std::endl;
            HandlerAcceptError(handler, error);
            return;
        }
        handlers_.insert(std::make_pair(handler->GetConnId(), handler));
        std::cout << "current connect count: " << handlers_.size() << std::endl;
        handler->HandleRead();
        Accept();
    });
}

void Server::HandlerAcceptError(std::shared_ptr<RWHandler> eventHandler, boost::system::error_code const& error) {
    std::cout << "Error, error reason:  "<< error.value() << error.message() << std::endl;
    eventHandler->CloseSocket();
    StopAccept();
}

void Server::StopAccept() {
    boost::system::error_code ec;
    acceptor_.cancel(ec);
    acceptor_.close(ec);
    ios_.stop();
}

std::shared_ptr<RWHandler> Server::CreateHandler() {
    int conn_id = conn_id_pool_.front();
    conn_id_pool_.pop_front();
    std::shared_ptr<RWHandler> handler = std::make_shared<RWHandler>(ios_);
    handler->SetConnId(conn_id);
    handler->SetCallbackError([this] (int conn_id) {
        RecyclConnId(conn_id);
    });
    return handler;
}

void Server::RecyclConnId(int conn_id) {
    auto iter = handlers_.find(conn_id);
    if (iter != handlers_.end()) 
        handlers_.erase(iter);
    std::cout << "current connect count: " << handlers_.size() << std::endl;
    conn_id_pool_.push_back(conn_id);
}

}
#endif