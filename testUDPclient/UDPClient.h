#ifndef UDPCLIENT_H_
#define UDPCLIENT_H_

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::asio::deadline_timer;
using boost::asio::ip::udp;

class UDPClient
{
public:
  UDPClient(
      boost::asio::io_service& io_service,
      const std::string& host,
      const std::string& port
  ) : io_service_(io_service),
      socket_(io_service, udp::endpoint(udp::v4(), 0)),
      deadline_(io_service_) {

    udp::resolver resolver(io_service_);
    udp::resolver::query query(udp::v4(), host, port);
    udp::resolver::iterator iter = resolver.resolve(query);
    endpoint_ = *iter;
    recvBuffer_.fill(0);
    deadline_.expires_at(boost::posix_time::pos_infin);
    check_deadline();

  }

  ~UDPClient()
  {
    socket_.close();
  }
  int runAsync(){
      this->io_service_.stop();
      this->io_service_.reset();
      try {
          boost::asio::io_service::work      work( this->io_service_ );
          boost::shared_ptr< boost::thread > _io_service_thread;
          _io_service_thread.reset( new boost::thread( boost::bind( &boost::asio::io_service::run, &this->io_service_) ) );
           return 0;
      }
      catch ( std::exception& e ) {
          std::cout << "RunAynsc error" << std::endl;
          sleep( 1 );
          return 1;
      }
  }

  void send(const std::string& msg);

  std::string receiveSync(boost::posix_time::time_duration timeout, boost::system::error_code& ec);
  void startReceiveAsync();
private:
  void check_deadline();

  static void handle_receive(const boost::system::error_code& ec, std::size_t length,
                                        boost::system::error_code* out_ec, std::size_t* out_length) {
    *out_ec = ec;
    *out_length = length;

  }
  void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

  boost::asio::io_service& io_service_;
  udp::socket socket_;
  udp::endpoint endpoint_;
  deadline_timer deadline_;
  std::array<char, 1024> recvBuffer_;
  std::string data_;
};

#endif
