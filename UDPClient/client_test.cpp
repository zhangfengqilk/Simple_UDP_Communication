#include "UDPClient.h"

int main()
{
    boost::asio::io_service io_service;
    boost::system::error_code ec;
    UDPClient client(io_service, "localhost", "1111");

    client.send("Hello, World! (Client)");
    std::string data = client.receive(boost::posix_time::seconds(10), ec);
    if (ec)
    {
        std::cout << "Receive error: " << ec.message() << "\n";
    }
    else
    {
        std::cout << "Received: ";
        std::cout << data;
        std::cout << "\n";
    }
}