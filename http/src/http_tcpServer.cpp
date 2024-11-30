#include "../include/http_tcpServer.hpp"

#include <iostream>
#include <sstream>


namespace http
{

    const int BUFFER_SIZE = 30720;

    /** @brief Initalize server
    * @param ip_address IP address of the server
    * @param port Port of the server
    * @post initalize socket and threadpool and platform specific APIs
    * @note due to different APIs between Windows and Linux, some macros are used to generate code based on targeting platform
    */
    TcpServer::TcpServer(std::string ip_address, int port ) : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(),
                                                             m_incomingMessage(),
                                                             m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress))
                                                             #ifdef WindowsOS
                                                             , m_wsaData()
                                                             #endif
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

    /*
    @throw exit with error in case of:
        - Cannot create socket
        - Cannot connect socket to address
    */
    int TcpServer::startServer()
    {
        //windows specific initialization
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

    /*
        closes the socket, the code uses platform specific APIs
    */
    void TcpServer::closeServer()
    {
        //since the function used platform specific APIs, it uses Macros to generate the appropriate API call based on the platform.
        #ifdef WindowsOS
            WSACleanup();
            closesocket(m_socket);
            closesocket(m_new_socket);
        #endif

        #ifdef LinuxOS
            close(m_socket);
            close(m_new_socket);
        #endif

        exit(0);
    }

    /*
    @brief an infinite loop listening and accepting new connections and logging
    */
    void TcpServer::startListening()
    {
        if (listen(m_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
        log(ss.str());

        int bytesReceived;
        while (true)
        {
            log("====== Waiting for a new connection ======\n\n\n");
            acceptConnection(m_new_socket);

            char buffer[BUFFER_SIZE] = {0};
            #ifdef WindowsOS
                bytesReceived = recv(m_new_socket, buffer, BUFFER_SIZE, 0);
            #else
                bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
            #endif
            
            if (bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }
            //overrided in the base, so that it calls its specific routing function
            get(buffer);
            
            close(m_new_socket);

        }
    }

    /*
    @brief accepting a new connection and bind to a new socket
    */
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

    //virtual function to handle new connections
    void TcpServer::get(const std::string& data){
        // std::ostringstream ss;
        // ss << "------ Received Request from client -base ------\n\n";
        // log(ss.str());
        // std::istringstream requestStream(data);
        // std::string method, endpoint;
        // requestStream >> method >> endpoint;
        
    }



    /*
    @brief a wrapper around cout << <<endl;
    */
    void TcpServer::log(const std::string &message)
    {
        #if DEBUG == 1
            std::cout << message << std::endl;
        #endif
    }

    /*
    @brief exit the program and printing the error reason
    @param errorMessage the message that will be logged
    */
    void TcpServer::exitWithError(const std::string &errorMessage)
    {
        #ifdef WindowsOS
            std::cout << WSAGetLastError() << std::endl;
        #endif
        log("ERROR: " + errorMessage);
        exit(1);
    }
   

}