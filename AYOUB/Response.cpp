

#include "Response.hpp"

void    Response::generateResponse(int &fd, Request *req)
{
    checkHeaders(req);
    // std::string
    if (req->get_method() == "GET")
    {
        std::cout << RED << "GET METHOD" << DEF << std::endl;
        GET(fd, req);
    }

    else if(req->get_method() == "POST")
    {
        std::cout << RED << "POST METHOD" << std::endl;
        POST(fd, req);
    }

    else if(req->get_method() == "DELETE")
    {
        std::cout << RED << "DELETE METHOD" << DEF << std::endl;
        DELETE(fd, req);
    }

    else
    {
        std::cout << RED << "UNKOWN METHOD" << DEF << std::endl;
    }

}

void Response::serv_file(map_iterator &type, int &fd, Request *req)
{

    std::cout << req->get_path() << std::endl;
    std::ifstream file(req->get_path().c_str());
    if(!file.is_open())
    {
        std::cout << " <  ---------- Error file --------->\n" << std::endl;
        throw (notFound());
    }
    else
    {
        std::cout << " <  ---------- YES --------->\n" << std::endl;
        std::string content = getResource(file, type->second);
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
        send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
    }
}

void	Response::serv_dir(int &fd, Request *req)
{
    std::string path = (req->get_path());
    if ( path == "./www/server1")
        throw (badRequest());
    if ( path == "./www/server1/")
    {
        std::cout << " <  ---------- home --------->\n" << std::endl;
        std::string content = homepage();
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
    }
}

void	Response::checkHeaders(Request *req)
{
    std::map<std::string, std::string> head;

    head = req->get_headers();
    if(head.count("Transfer-Encoding:") && req->get_header("Transfer-Encoding:") != "chunked")
        throw(notImplement());
    if(req->get_method() == "POST" && !head.count("Transfer-Encoding:") && !head.count("Content-Length:"))
        throw(badRequest());
    if((req->get_method() == "GET") && ( !req->get_body().empty() || head.count("Content-Length:")))
        throw(badRequest());
    if(head.count("Transfer-Encoding:") && head.count("Content-Length:"))
        throw(badRequest());
    if(req->get_protocol().empty() || req->get_protocol() != "HTTP/1.1")
        throw(httpVersion());
    if( req->body_limit < atoi(req->get_header("Content-Length:").c_str()))
        throw(EntityTooLarge());
    if(req->get_path().size() > 2048)
        throw (longRequest());
}


Response::Response()
{
}

Response::~Response()
{
}


std::string Response::getResource(std::ifstream &file, std::string &type)
{
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: "<< type << "\r\n"
            << "Connection: close\r\n"
            << "Server: " << "chabchoub" << "\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n"
            << "Content-Length: "<< buffer.size() <<"\r\n"
            << "\r\n"
            << buffer;
    return response.str();
}

std::string Response::getImage(std::ifstream &file, const char *type, std::string ext)
{
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: "<< type << (ext == "svg" ? "svg+xml" : ext) << "\r\n"
            << "Connection: close\r\n"
            << "Server: " << "chabchoub" << "\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n"
            << "Content-Length: "<< buffer.size() <<"\r\n"
            << "\r\n"
            << buffer;
    return response.str();
}


std::string Response::extension(const std::string &path)
{
    size_t dot = path.rfind('.');
    if (dot == std::string::npos)
    {
        std::cerr << RED<<"failure to find extension !"<<DEF << std::endl;
        return "";
    }
    return path.substr(dot + 1);
}



// TODO <=====================              Error pages                 ================================>


std::string Response::homepage()
{
    std::ifstream file((SERVER_ROOT + std::string("/index.html")).c_str());
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        throw(notFound());
    }
    std::cout << "the file is excite \n" <<std::endl;
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::notFound()
{
    srand(time(NULL));
    int f = rand() % 2;
    const char *path[] = {"www/Errors/404.html", "www/Errors/404-2.html"} ;
    std::ifstream file(path[f]);
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in 404 page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 404 Not Found\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::notImplement()
{
    std::ifstream file("www/Errors/501.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in 501 page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 501 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}


std::string Response::httpVersion()
{
    std::ifstream file("www/Errors/505.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in 505 page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 501 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::EntityTooLarge()
{
    std::ifstream file("www/Errors/413.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 413 Entity too large\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::forbidden()
{
    std::ifstream file("www/Errors/403.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in 403 page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 403 forbidden\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::badRequest()
{
    std::ifstream file("www/Errors/400.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 400 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::longRequest()
{
    std::ifstream file("www/Errors/414.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 414 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}



//TODO /===================================================================================================/





// std::string Response::Redirect()
// {
    
//     std::stringstream response;
//     response << "HTTP/1.1 301 Moved Permanently\r\n"
//              << "Content-Type: "<< type << ext << "\r\n"
//              << "Content-Length: "<< buffer.size() <<"\r\n"
//              << "Location: "
//              << "\r\n\r\n"
//              << buffer.c_str();
//     return response.str();
// }


std::string getCurrentDateTime() {
    char buffer[80];
    std::time_t rawtime;
    std::tm* timeinfo;

    std::time(&rawtime);
    timeinfo = std::gmtime(&rawtime);

    // Format the date and time
    std::strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return std::string(buffer);
}


bool directoryExists(std::string path) {
    struct stat info;

    // Use stat to check if the directory exists
    if (stat(path.c_str(), &info) != 0) {
        // If stat returns non-zero, the directory doesn't exist
        return false;
    }

    // Check if it is a directory
    return (info.st_mode & S_IFDIR) != 0;
    // S_IFREG
}

bool fileExists(std::string path) {
    struct stat info;

    // Use stat to check if the directory exists
    if (stat(path.c_str(), &info) != 0) {
        // If stat returns non-zero, the directory doesn't exist
        return false;
    }

    // Check if it is a directory
    return (info.st_mode & S_IFREG) != 0;
    // S_IFREG
}