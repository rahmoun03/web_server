#ifndef CLIENT
#define CLIENT
#include <sstream>
// #include "../CHEBCHOUB/WebServer.hpp"
#include "Request.hpp"
#include "Response.hpp"
// class Response;
// class Request;


class Client
{
    public:
        std::stringstream buf;
        bool toRespons;
        int server_index;
        size_t endOf;
        Request req;
        Response res;
        
        
        Client(/* args */);
        void clear();
        ~Client();

};

#endif
