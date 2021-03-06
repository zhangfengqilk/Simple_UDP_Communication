#include "UDPClient.h"

void UDPClient::send(const std::string& msg) {
        socket_.send_to(boost::asio::buffer(msg, msg.size()), endpoint_);
}

std::string UDPClient::receiveSync(boost::posix_time::time_duration timeout, boost::system::error_code& ec) {
        deadline_.expires_from_now(timeout);
        ec = boost::asio::error::would_block;
        std::size_t length = 0;

        socket_.async_receive(boost::asio::buffer(recvBuffer_),
                                                boost::bind(&UDPClient::handle_receive, _1, _2, &ec, &length));

        do io_service_.run_one(); while (ec == boost::asio::error::would_block);

        data_ = std::string(std::begin(recvBuffer_), std::end(recvBuffer_));
        recvBuffer_.fill(0);
        return data_;
}

void UDPClient::startReceiveAsync()
{
    if(this->io_service_.stopped()){
        this->runAsync();
    }
    socket_.async_receive(boost::asio::buffer(recvBuffer_),boost::bind(&UDPClient::handleReceive,this, _1, _2));
    data_ = std::string(std::begin(recvBuffer_), std::end(recvBuffer_));
    recvBuffer_.fill(0);
    std::cout << data_ << std::endl;
}

void UDPClient::check_deadline() {
        if (deadline_.expires_at() <= deadline_timer::traits_type::now())
        {
                socket_.cancel();
                deadline_.expires_at(boost::posix_time::pos_infin);
        }

        deadline_.async_wait(boost::bind(&UDPClient::check_deadline, this));
}

void UDPClient::handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error || error == boost::asio::error::message_size) {
          auto message = std::make_shared<std::string>("Hello, World! (Server)\n");
          startReceiveAsync();
    }
 }

