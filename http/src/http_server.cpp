#include "../include/http_server.hpp"
#include "../include/http_tcpServer.hpp"


namespace http{
    
    Server::Server(std::string ip_address, int port) : TcpServer(ip_address, port) {}

    void Server::startServer(){
        startListening();
    }

    /*
    @brief handling get requests
    @param data: incoming data from client
    filtering the data to figure out the  endpoint and the method
    @post execute the appropriate method -that the developer has registered using registerEndpoint()- in a thread from threadpool
    */
    void Server::get(const std::string& data) 
    {   
        std::ostringstream ss;
        ss << "------ Received Request from client------\n\n";
        ss << data << "\n\n------ End of Request ------\n";
        //log(ss.str());
          
        std::istringstream requestStream(data);
        std::string method, endpoint;
        requestStream >> method >> endpoint;
        std::string filterdData  = filterData(data);
        log("Received data:");
        log(filterdData);
        m_threadpool.addTask(
            [this, endpoint, method, filterdData]
            (){
                route(endpoint, method, filterdData);
            }
        );
        
     
    }
    
    /*
    @brief handle post request
    */
    void Server::post(std::string body)
    {
        long bytesSent;
        //wrap header with the passed body
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

    /*
    @brief filter the 9-lines of header and return the body
    */
    std::string Server::filterData(std::string data) {
        int pos = -1 ;
        int tempPos ;
        for (int i = 0 ; i < 9 ; i++){ //skip 9 lines
            tempPos = data.find("\n",pos + 1);
            if (tempPos != -1){
                pos = tempPos;
            }
        }
        return data.substr(pos + 1, data.length()  - pos);
    }


    /*
    @brief build the response to the client with the provided body
    */
    std::string Server::buildResponse( std::string response)
    {
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << response.size() << "\n\n"
           << response;

        return ss.str();
    }

    /*
    @brief register a function to be executed if the client requrests the endpoint with the required method
    @param endpoint: the endpoint requested by the client
    @param method: the method requested by the client (GET/POST)
    @param callback: the function to be executed when the client requests the endpoint with the required method
    */
    void Server::registerEndpoint(const std::string& endpoint, const std::string& method, std::function<void(const std::string& data)> callback) {
            m_endpoints[endpoint][method] = callback;
    }
    
    /*
    @brief execute the appropriate callback
    @param endpoint: the endpoint requested by the client
    @param method: the method requested by the client (GET/POST)
    @param data: the body sent by the client (if the method is POST) or the header sent by the client (if the method is GET)
    */
    void Server::route(std::string endpoint, std::string method, std::string data)
    {
        //execute the crossponding function for the current endpoint with method (get/post)
        if (m_endpoints.find(endpoint) != m_endpoints.end() && m_endpoints[endpoint].find(method) != m_endpoints[endpoint].end()) {
            m_endpoints[endpoint][method](data);
        } 
    }

    /*
    @brief change the number of working threads
    */
    void Server::resetThreadPoolNumber(int newNum) {
        //actual implementation is abstracted, review the threadpool project documentation to find its implementation
        m_threadpool.expandPool(newNum);
    }
}

