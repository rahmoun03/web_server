#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"

class Response
{
private:
    /* data */
public:
    Response();
    Response(int &fd, Request *req);
    std::string notFound();
    std::string homepage();
    std::string getResource(int &fd);
    ~Response();
};

#endif