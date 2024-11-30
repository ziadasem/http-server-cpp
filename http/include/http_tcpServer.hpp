#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX

#include <stdio.h>
#include "../dependencies/threadpool/include/threadpool.hpp"


#ifdef LinuxOS
    #include <sys/socket.h>
    #include <arpa/inet.h>
#endif


#ifdef WindowsOS
    #include <winsock.h>
#endif


#include <stdlib.h>
#include <string>
#include <unistd.h>

namespace http
{

    class TcpServer
    {
    public:
        TcpServer(std::string ip_address, int port);
        ~TcpServer();
        
    protected:
        int m_new_socket;
        
        void startListening( );
        void log(const std::string &message);
        void exitWithError(const std::string &errorMessage);
        virtual void get(const std::string& data);
    private:
        std::string m_ip_address;
        int m_port;
        int m_socket;
        
        long m_incomingMessage;
        struct sockaddr_in m_socketAddress;
        int m_socketAddress_len;
        std::string m_serverMessage;
        #ifdef WindowsOS
            WSADATA m_wsaData;
        #endif

        int startServer();
        void closeServer();
        void acceptConnection(int &new_socket);
    };


    

} // namespace http

#endif