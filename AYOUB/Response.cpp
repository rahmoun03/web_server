

#include "Response.hpp"

void    Response::generateResponse(int &fd, Request *req)
{
    checkHeaders(req);
    if (req->get_method() == "GET")
    {
        std::cout << RED << "GET METHOD" << DEF << std::endl;
        if(((extension(req->get_path())) == "jpeg") || ((extension(req->get_path())) == "svg") || ((extension(req->get_path())) == "png"))
            imageFile(fd, req);
        else
            htmlFile(fd, req);
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
}


Response::Response()
{
}

Response::~Response()
{
}


std::string Response::getResource(std::ifstream &file, const char *type, std::string ext)
{
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: "<< type << ext << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer.c_str();
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

void	Response::htmlFile(int &fd, Request *req)
{
    if ((req->get_path()) == "/")
    {
        std::cout << " <  ---------- home --------->\n" << std::endl;
        std::string content = homepage();
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
    }
    else if (req->get_path().empty())
    {
        throw (badRequest());
    }
    else if(req->get_path().size() > 2048)
        throw (longRequest());
    else
    {
        std::string path = (SERVER_ROOT + req->get_path());
        std::cout << path << std::endl;
        std::ifstream file(path.c_str());
        if(!file.is_open())
        {
            std::cout << " <  ---------- Error file --------->\n" << std::endl;
            throw (notFound());
        }
        else
        {
            std::cout << " <  ---------- YES --------->\n" << std::endl;
            std::string content = getResource(file, "text/", extension(req->get_path()));
            std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
            send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
        }
    }
}
void	Response::imageFile(int &fd, Request *req)
{
    std::string path = (IMG_ASSET + req->get_path());
        std::cout << path << std::endl;
    
    std::ifstream img(path.c_str());
    if(!img.is_open())
    {
        std::cout << " <  ---------- Error image --------->\n" << std::endl;
        throw(notFound());

    }
    else
    {
        std::string content = getImage(img, "image/", extension(req->get_path()));
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content << std::endl;
        send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
    }

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
// #include <sys/stat.h>
// bool directoryExists(const char* path) {
//     struct stat info;

//     // Use stat to check if the directory exists
//     if (stat(path, &info) != 0) {
//         // If stat returns non-zero, the directory doesn't exist
//         return false;
//     }

//     // Check if it is a directory
//     return (info.st_mode & S_IFDIR) != 0;
    // S_IFREG
// }