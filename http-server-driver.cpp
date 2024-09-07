#include "http/include/http_server.hpp"

#include <iostream>
#include <chrono>



int main(int argC, char **argv)
{
    http::Server server = http::Server(argv[1], 8081); //bind to local IP
    auto handleDefualtRoute = [&server](const std::string& s) { 
            
            server.post("<h1>Hello from Server</h1>");
            //std::this_thread::sleep_for(std::chrono::seconds(5)); //simulate some delay
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
