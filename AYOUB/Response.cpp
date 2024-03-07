

#include "Response.hpp"

void    Response::generateResponse(int &fd, Request *req)
{
    checkHeaders(req);
    if (req->get_method() == "GET")
    {
        std::cout << RED << "GET METHOD" << DEF << std::endl;
        std::map<std::string, std::string> map = ErrorAssets();
        map_iterator it = map.find(req->get_path());

        if(it != map.end())
            req->get_path() = it->second;
        else
            req->get_path() = (SERVER_ROOT + req->get_path());

        if(directoryExists(req->get_path()))
        {
            std::cout << "the URL is a directory \n";
            serv_dir(fd, req);
        }
        else if (fileExists(req->get_path()))
        {
            std::map<std::string , std::string> mime_map = mimeTypes();
            map_iterator it = mime_map.find(extension(req->get_path()));
            if(it != mime_map.end())
            {
                std::cout << "the URL is a file : " << it->second << std::endl;
                serv_file(it, fd, req);
            }
            else
            {
                std::cout << "NOT FOUND 404"<< std::endl;
                throw(notFound());
            }
        }
        else
        {
            std::cout << "NOT FOUND 404"<< std::endl;
            throw(notFound());
        }
    }


    else if(req->get_method() == "POST")
    {
        std::cout << RED << "POST METHOD" << std::endl;
    }

    else if(req->get_method() == "DELETE")
    {
        std::cout << RED << "DELETE METHOD" << DEF << std::endl;
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
    if ((req->get_path()) == "./www/server1")
        throw (badRequest());
    if ((req->get_path()) == "./www/server1/")
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



/*                        Error pages                             */


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
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::notFound()
{
    srand(time(NULL));
    int f = rand() % 2;
    const char *path[] = {"Errors/404.html", "Errors/404-2.html"} ;
    std::ifstream file(path[f]);
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 404 Not Found\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::notImplement()
{
    std::ifstream file("Errors/501.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 501 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::EntityTooLarge()
{
    std::ifstream file("Errors/413.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 413 Entity too large\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::badRequest()
{
    std::ifstream file("Errors/400.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 400 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}

std::string Response::longRequest()
{
    std::ifstream file("Errors/414.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
    }
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 414 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
    return response.str();
}



/*******************************************************************************************/





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
#include <sys/stat.h>
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