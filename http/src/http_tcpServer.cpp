#include "../headers/http_tcpServer.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>


namespace http
{

    const int BUFFER_SIZE = 30720;

    TcpServer::TcpServer(std::string ip_address, int port ) : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(),
                                                             m_incomingMessage(),
                                                             m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress))
                                                             #ifdef WindowsOS
                                                             , m_wsaData()
                                                             #endif
                                                             //m_serverMessage(buildResponse("<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>"))
    {
        m_socketAddress.sin_family = AF_INET;
        m_socketAddress.sin_port = htons(m_port);
        m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

        if (startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
            log(ss.str());
        }

    }

    TcpServer::~TcpServer()
    {
        closeServer();
    }

    int TcpServer::startServer()
    {
#ifdef WindowsOS
        if (WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0)
        {
            exitWithError("WSAStartup failed");
        }
#endif

        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0)
        {
            exitWithError("Cannot create socket");
            return 1;
        }

        if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }

        return 0;
    }

    void TcpServer::closeServer()
    {
        
        close(m_socket);
        close(m_new_socket);
        #ifdef WindowsOS
            WSACleanup();
        #endif
        exit(0);
    }

    void TcpServer::startListen()
    {
        if (listen(m_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
        log(ss.str());

        int bytesReceived;
        ThreadPool threadpool;
        while (true)
        {
            log("====== Waiting for a new connection ======\n\n\n");
            acceptConnection(m_new_socket);

            char buffer[BUFFER_SIZE] = {0};
            #ifdef WindowsOS
                bytesReceived = recv(m_new_socket, buffer, BUFFER_SIZE, 0);
            #elif
                bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
            #endif
            
            if (bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }
            get(buffer);

            close(m_new_socket);
        }
    }

    void TcpServer::acceptConnection(int &new_socket)
    {
        new_socket = accept(m_socket, (sockaddr *)&m_socketAddress, (int *)&m_socketAddress_len);
        if (new_socket < 0)
        {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " << ntohs(m_socketAddress.sin_port);
            exitWithError(ss.str());
        }
    }

    //virtual
    void TcpServer::get(const std::string& data){
         std::ostringstream ss;
        ss << "------ Received Request from client ------\n\n";
        log(ss.str());
        std::istringstream requestStream(data);
        std::string method, endpoint;
        requestStream >> method >> endpoint;
        
    }




    void TcpServer::log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void TcpServer::exitWithError(const std::string &errorMessage)
    {
        #ifdef WindowsOS
            std::cout << WSAGetLastError() << std::endl;
        #endif
        log("ERROR: " + errorMessage);
        exit(1);
    }
   

} // namespace http