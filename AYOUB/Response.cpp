/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arahmoun <arahmoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:39:33 by arahmoun          #+#    #+#             */
/*   Updated: 2024/02/26 15:14:11 by arahmoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

Response::Response(int &fd, Request *req)
{
    if(((extension(req->get_path())) == "jpeg") || ((extension(req->get_path())) == "png"))
        imageFile(fd, req);
    else
        htmlFile(fd, req);
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
            std::cout << " <  ---------- Error --------->\n" << std::endl;
            std::string content = notFound();
            std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
            send(fd, content.c_str(), content.size(), 0);
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
    std::ifstream img(req->get_path().c_str(), std::ios::binary);
    if(!img.is_open())
    {
        std::cout << " <  ---------- Error --------->\n" << std::endl;
        std::string content = notFound();
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        send(fd, content.c_str(), content.size(), 0);
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

