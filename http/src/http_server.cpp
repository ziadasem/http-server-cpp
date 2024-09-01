#include "../headers/http_server.hpp"
#include "../headers/http_tcpServer.hpp"

namespace http{
    
    Server::Server(std::string ip_address, int port) : TcpServer(ip_address, port) {}

    void Server::startServer(){
        startListen();
    }

    void Server::get(const std::string& data) 
    {   
        std::ostringstream ss;
        ss << "------ Received Request from client------\n\n";
        log(ss.str());
       
            
        std::istringstream requestStream(data);
        std::string method, endpoint;
        requestStream >> method >> endpoint;
        std::string filterdData  = filterData(data);
    
        m_threadpool.addTask(
            [this, endpoint, method, filterdData]
            (){
                route(endpoint, method, filterdData);
            }
        );
        
     
    }

    std::string Server::filterData(std::string data){
        int pos = -1 ;
        for (int i = 0 ; i < 9 ; i++){ //skip 9 lines
            pos = data.find("\r\n",pos + 2);
        }
        return data.substr(pos + 1, data.length()  - pos);

    }

    void Server::post(std::string body)
    {
        long bytesSent;
        
        std::string _body = buildResponse(body);
        #ifdef WindowsOS
            bytesSent = send(m_new_socket, _body.c_str(), _body.size(), 0);
        #else
            bytesSent = write(m_new_socket, _body.c_str(), _body.size());
        #endif
        if (bytesSent == _body.size())
        {
            log("------ Server Response sent to client ------\n\n");
        }
        else
        {
            log("Error sending response to client");
        }
    }


    std::string Server::buildResponse( std::string response)
    {
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << response.size() << "\n\n"
           << response;

        return ss.str();
    }

    void Server::registerEndpoint(const std::string& endpoint, const std::string& method, std::function<void(const std::string& data)> callback) {
            m_endpoints[endpoint][method] = callback;
    }
    
    void Server::route(std::string endpoint, std::string method, std::string data)
    {
        //execute the crossponding function for the current endpoint with method (get/post)
        if (m_endpoints.find(endpoint) != m_endpoints.end() && m_endpoints[endpoint].find(method) != m_endpoints[endpoint].end()) {
            m_endpoints[endpoint][method](data);
        } 
    }
}
