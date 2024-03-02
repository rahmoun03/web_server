

#include "Response.hpp"

void    Response::generateResponse(int &fd, Request *req)
{
    if (req->get_method() == "GET")
    {
        std::cout << RED << "GET METHOD" << DEF << std::endl;

        if(((extension(req->get_path())) == "jpeg") || ((extension(req->get_path())) == "png"))
            imageFile(fd, req);
        else if (req->get_path() == SERVER_ROOT)
            throw "bad request 400";
        else
            htmlFile(fd, req);
    }
    else if(req->get_method() == "POST")
    {
        std::string path = ("./assets/upload/img." + req->get_header("Content-Type:").substr(7));
        std::cout << RED << "POST METHOD\n" << DEF << "path : "<< path << std::endl;
        std::ofstream out(path.c_str(), std::ios::binary);
        out << req->get_body();
        std::cout << "lenght : " << req->get_body().size()<< std::endl;
        std::cout << "the image was created" << std::endl;
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

Response::Response()
{
}

Response::~Response()
{
}

std::string Response::homepage()
{
    std::ifstream file("./assets/index.html");
    if(!file.is_open())
    {
        std::cerr << RED <<"failure in home page" << std::endl;
        exit(1);
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
    std::ifstream file("assets/notFound.html");
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
             << "Content-Type: "<< type << ext << "\r\n"
             << "Content-Length: "<< buffer.size() <<"\r\n"
             << "\r\n"
             << buffer;
    return response.str();
}

void	Response::htmlFile(int &fd, Request *req)
{
    if (req->get_path() == ("./assets/"))
    {
        std::cout << " <  ---------- home --------->\n" << std::endl;
        std::string content = homepage();
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        send(fd, content.c_str(), content.size(), 0);
    }
    else
    {
        std::ifstream file(req->get_path().c_str());
        if(!file.is_open())
        {
            std::cout << " <  ---------- Error file --------->\n" << std::endl;
            throw (notFound());
            // std::string content = ;
            // std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
            // send(fd, content.c_str(), content.size(), 0);
        }
        else
        {
            std::cout << " <  ---------- YES --------->\n" << std::endl;
            std::string content = getResource(file, "text/", extension(req->get_path()));
            std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
            send(fd, content.c_str(), content.size(), 0);
        }
    }
}
void	Response::imageFile(int &fd, Request *req)
{
    std::ifstream img(req->get_path().c_str());
    if(!img.is_open())
    {
        std::cout << " <  ---------- Error image --------->\n" << std::endl;
        throw( notFound());
        // std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        // send(fd, content.c_str(), content.size(), 0);
    }
    else
    {
        std::string content = getImage(img, "image/", extension(req->get_path()));
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content << std::endl;
        send(fd, content.c_str(), content.size(), 0);
    }

}


std::string Response::extension(const std::string &path)
{
    size_t dot = path.rfind('.');
    if (dot == std::string::npos)
    {
        std::cerr << RED<<"failure to find extension !" << std::endl;
        exit(1);
    }
    return path.substr(dot + 1);
}

