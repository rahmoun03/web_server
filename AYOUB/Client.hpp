#ifndef CLIENT
#define CLIENT
#include "Request.hpp"
#include "Response.hpp"
// class Response


class Client
{
    public:
        std::stringstream buf;
        bool toRespons;
        size_t endOf;
        Request req;
        Response res;
        
        Client(/* args */);
        void clear();
        ~Client();
};


void Client::clear()
{
    buf.str("");
    toRespons = false;
    endOf = -1;
    req.clear();
    res.clear();
    std::cout << RED <<"clear Client object" << DEF<< std::endl;
}

Client::Client(/* args */)
{
}

Client::~Client()
{
}


#endif
