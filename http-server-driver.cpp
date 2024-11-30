#include "http/include/http_server.hpp"

#include <iostream>
#include <chrono>



int main(int argC, char **argv)
{
    std::string ipaddress ;
    int port ;
    if (argC != 3){
        std::cerr << "[Warning]: No IP and port number are enterd. use default IP:PortNumber 127.0.0.1:8081" << std::endl;
        ipaddress = const_cast<char*>("127.0.0.1");
        port = 8081;
    }else{
        ipaddress = argv[1];
        port = std::stoi(argv[2]);
    }
    http::Server server = http::Server(ipaddress, port); //bind to local IP
    auto handleDefualtRoute = [&server](const std::string& s) { 

            server.post("<h1>Hello from Server " + s + "</h1>" );
            std::this_thread::sleep_for(std::chrono::seconds(5)); //simulate some delay
    };
    
    
    using namespace http;
    
    //register end points for the server
    server.registerEndpoint(
        "/",
        "POST",
        handleDefualtRoute
    );

     server.registerEndpoint(
        "/",
        "GET",
        handleDefualtRoute
    );
    
    server.startServer();

    return 0;
}
