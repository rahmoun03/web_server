
#include "Response.hpp"

Response::Response(int &fd, Request *req)
{
    int file = open(req->get_path().c_str(), O_RDONLY);
    if(file == -1)
        if (req->get_path() == "./")
            send(fd, homepage().c_str(), homepage().size(), 0);
        else
            send(fd, notFound().c_str(), notFound().size(), 0);
    else
    {
        std::string content = getResource(fd);
        send(fd, content.c_str(), content.size(), 0);
    }
}

Response::Response()
{
}

Response::~Response()
{
}

std::string Response::homepage()
{
    int file = open("assets/home.html", O_RDONLY);
    char *buffer = NULL;
   
    std::stringstream content;
    while (true)
    {
        ssize_t a = read(file, buffer, 1024);
        if(a == -1)
            std::cerr << "fialure in read" << std::endl;
        else if (a < 1024)
        {
            buffer[a] = '\0';
            content << buffer;
            break;
        }
        else
        {
            buffer[a] = '\0';
            content << buffer;
        }
    }
    std::stringstream response;
    response << "HTTP/1.1 404 Not Found\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< content.str().size() <<"\r\n"
             << "\r\n"
             << content.str();
    return response.str();
}

std::string Response::notFound()
{
    int file = open("assets/notFound.html", O_RDONLY);
    char buffer[1024];
    ssize_t a;
    std::stringstream content;
    while ((a = read(file, buffer, sizeof(buffer))) > 0)
    {
        buffer[a] = '\0';
        content << buffer;
    }
    std::stringstream response;
    response << "HTTP/1.1 404 Not Found\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< content.str().size() <<"\r\n"
             << "\r\n"
             << content.str();
    return response.str();
}

std::string Response::getResource(int &fd)
{
    char buffer[1024];
    ssize_t a;
    std::stringstream content;
    while ((a = read(fd, buffer, sizeof(buffer))) > 0)
    {
        buffer[a] = '\0';
        content << buffer;
    }
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< content.str().size() <<"\r\n"
             << "\r\n"
             << content.str();
    return response.str();
}
