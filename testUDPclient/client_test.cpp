#include "UDPClient.h"
#include "unistd.h"
#include <thread>

typedef boost::posix_time::time_duration Timeout;
int main()
{
    boost::asio::io_service io_service;
    boost::system::error_code ec;
    UDPClient client(io_service, "localhost", "1111");    
//    client.runAsync();
//    client.startReceiveAsync();
    std::thread t([&](){
        while(1){
            std::string localReceiveSync = client.receiveSync(boost::posix_time::seconds(10),ec);
            std::cout<<localReceiveSync<<std::endl;
        }

    });
    t.detach();
    while(1){
        client.send("Hello, World! (Client)");
        sleep(1);
    }

    return 0;
}
