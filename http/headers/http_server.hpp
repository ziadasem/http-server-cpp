#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

//Extending http_server to make the API agnostic about connection protocol (TCP/UDP)
#include "http_tcpServer.hpp"
#include <map>
#include <string>


#ifdef LinuxOS
    #include <unistd.h>
#endif


#include <sstream>


namespace http
{
   
class Server  : public TcpServer {
   public:
        Server(std::string ip_address, int port);
        ~Server(){}
        void post(std::string body);
        void startServer();        
        void registerEndpoint(const std::string& endpoint, const std::string& method, std::function<void(const std::string& data)> callback);
        void get(const std::string& data);
    private:
        ThreadPool m_threadpool{1};
        std::map<std::string, std::map<std::string, std::function<void(const std::string& data)> >> m_endpoints;

        std::string filterData(std::string data);
        std::string buildResponse( std::string response);
        void route(std::string endpoint, std::string method, std::string data);
      
};
};

#endif