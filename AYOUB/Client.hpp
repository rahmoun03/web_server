#ifndef CLIENT 
#define CLIENT
#include "../AYOUB/Request.hpp"
#include "../AYOUB/Response.hpp"

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
}

Client::Client(/* args */)
{
}

Client::~Client()
{
}


#endif