/**
 * 
 * 2019-05-25 by Manlou Hua(hua7th@gmail.com)
 * 
 * **/

#ifndef HTTP_PROXY_SRC_RWHANDLER_HPP
#define HTTP_PROXY_SRC_RWHANDLER_HPP

#include <array>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/array.hpp>

namespace ml {

const int MAX_IP_PACK_SIZE = 65536;
const int HEAD_LEN = 4;

class RWHandler {
public:
    RWHandler(boost::asio::io_service& ios);
    ~RWHandler();
    void HandleRead();
    void HandleWrite(char* data, int len);
    boost::asio::ip::tcp::socket& GetSocket();
    void CloseSocket();
    void SetConnId(int conn_id);
    int GetConnId() const;
    template<typename F>
    void SetCallbackError(F f);
private:
    void HandleError(boost::system::error_code const& error);
private:
    boost::asio::ip::tcp::socket tcp_socket_;
    boost::array<char, MAX_IP_PACK_SIZE> buffer_;
    int conn_id_;
    std::function<void(int)> callback_error_;
};

RWHandler::RWHandler(boost::asio::io_service& ios) : tcp_socket_(ios), callback_error_(nullptr) {
}

RWHandler::~RWHandler() {
}

void RWHandler::HandleRead() {
    boost::asio::async_read(tcp_socket_, boost::asio::buffer(buffer_), boost::asio::transfer_at_least(HEAD_LEN), 
        [this](boost::system::error_code const& error, size_t size) {
            if (error) {
                HandleError(error);
                return;
            }
            std::cout << buffer_.data() + HEAD_LEN << std::endl;
            HandleRead();
        });
}

void RWHandler::HandleWrite(char* data, int len) {
    boost::system::error_code error;
    boost::asio::write(tcp_socket_, boost::asio::buffer(data, len), error);
    if (error)
        HandleError(error);
}

boost::asio::ip::tcp::socket& RWHandler::GetSocket() {
    return tcp_socket_;
}

void RWHandler::CloseSocket() {
    boost::system::error_code ec;
    tcp_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
    tcp_socket_.close(ec);
}

void RWHandler::SetConnId(int conn_id) {
    conn_id_ = conn_id;
}

int RWHandler::GetConnId() const {
    return conn_id_;
}

template<typename F>
void RWHandler::SetCallbackError(F f) {
    callback_error_ = f;
}

void RWHandler::HandleError(boost::system::error_code const& error) {
    CloseSocket();
    std::cout << error.message() << std::endl;
    if (callback_error_) 
        callback_error_(conn_id_);
}

}
#endif