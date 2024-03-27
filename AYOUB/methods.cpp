#include "Response.hpp"
#include <filesystem>
#include<stdio.h>
#include<sys/stat.h>
#include<iostream>
#include<string.h>
#include<string>
#include <stdlib.h>
#include<dirent.h>
#include <iostream>
#include <cstdlib> // For system function
#include <unistd.h>

void	Response::GET(int &fd, Request &req, Conf &server)
{
    if(!req.red_path.empty())
            Redirect(req.red_path, req, fd);
    else if(directoryExists(req.get_path()))
    {
        // std::cout << "http://{" << req.get_path() << "} \n";
        // std::cout << "the URL is a directory \n";
        serv_dir(fd, req, server);
    }
    else if (fileExists(req.get_path()))
    {
        std::map<std::string , std::string> mime_map = mimeTypes();
        map_iterator it = mime_map.find(extension(req.get_path()));
        if(it != mime_map.end()) 
        {
            // std::cout << "http://{" << req.get_path() << "} \n";
            // std::cout << "the URL is a file : " << it->second << std::endl;
            serv_file(it, fd, req);
        }
        else if (extension(req.get_path()) == "php" && server.locat.find(req.locationPath)->second.cgi)
        {
            serveCgi(req.get_path());
            std::ifstream ff("./cgi_output.txt");
            std::stringstream response;
            response << "HTTP/1.1 411 Length Required\r\n"
                    << "Content-Type: text/plain\r\n"
                    << "Connection: close\r\n"
                    << "Server: chabchoub\r\n"
                    << "Date: " << getCurrentDateTime() << "\r\n";
            std::string res = std::string(std::istreambuf_iterator<char>(ff), std::istreambuf_iterator<char>()); 
            response << "Content-Length: " << res.size() << "\r\n"
                << "\r\n"
                << res;
            std::cout << "response : \n" << response.str() << std::endl;
            send(fd, response.str().c_str() , response.str().size(), 0);
            req.connexion = true;
        }
        else
        {
            std::cout << "NOT FOUND 404 in MimeTypes"<< std::endl;
            throw(notFound());
        }
    }
    else
    {
        std::cout << "NOT FOUND 404"<< std::endl;
        throw(notFound());
    }
}

unsigned long convertHexToDec(std::string hex)
{
    unsigned long decimal;
    std::stringstream ss;

    ss << hex;
    ss >> std::hex >> decimal;
    return (decimal);
}


void	Response::POST(int &fd, Request &req, Conf &server)
{

    (void) fd;
    (void) req;
    // netPlix net;
    static int i;
    (void) server;
    if(req.get_header("Transfer-Encoding:").empty())
    {
        if (req.firstTime)
        {
            std::string up_ptah = server.locat.find(req.locationPath)->second.upload;
            std::string type = static_cast<std::string>(req.get_header("Content-Type:"));
            std::string tmp_ = type.substr(type.find("/") + 1);
            std::cout << tmp_ << std::endl;
            type.erase(type.find("/"));
            type.push_back('.');
            path = up_ptah + ("upload." + tmp_);
            while (fileExists(path))
            {
                i++;
                std::stringstream ss;
                ss << i;
                std::string s;
                ss >> s;
                path = up_ptah + ("upload" + s + (".") + tmp_);
            }
            std::string str = req.get_body();
            out.open(path.c_str(), std::ios::binary);
            out.write(str.c_str(), str.size());
            out.flush();
            req.firstTime = false;
            std::cout << "++++++++++++++ HERE  +++++++++++++++" << std::endl;
            
        }
        else 
        {
            size_t a;
            char buffer[1024];
            a = recv(fd, buffer, 1023, 0);
            req.ra += a;
            buffer[a] = '\0';
            out.write(buffer, a);
            out.flush();
        }
        if(req.ra >= (size_t )atof(req.get_header("Content-Length:").c_str()))
        {
            req.connexion = true;
             std::ifstream fi("www/server1/suc.html");
            std::stringstream response;
            response << "HTTP/1.1 201 Created\r\n"
                    << "\r\n"
                    << fi.rdbuf();
            fi.close();
            send(fd, response.str().c_str(), response.str().size(),0);
        }
    }
    else if(req.get_header("Transfer-Encoding:") == "chunked")
    {
        std::string line; 
        std::cout << "------------BEFRO POST ------------------------------------\n";
        if (req.firstTime)
        {
            std::cout << "request path : " << req.get_path() << std::endl;
            std::string up_ptah = server.locat.find(req.locationPath)->second.upload;
            std::string type = static_cast<std::string>(req.get_header("Content-Type:"));
            std::string tmp_ = type.substr(type.find("/") + 1);
            std::cout << tmp_ << std::endl;
            type.erase(type.find("/"));
            type.push_back('.');
            std::string path = up_ptah + ("upload." + tmp_);
            std::cout << "path : "<< path << std::endl;

            while (fileExists(path))
            {
                i++;
                std::stringstream ss;
                ss << i;
                std::string s;
                ss >> s;
                path = up_ptah + ("upload" + s + (".") + tmp_);
            }
            out.open(path.c_str(), std::ios::binary);
            str = req.get_body();
            std::istringstream f(str);
            std::getline(f, line);
            if(str.substr(str.size() - 5, str.size()) == "0\r\n\r\n")
            {
                std::cout << "the body is finish\n";
                decimal = 0;
                str.erase(0,line.size() + 1);
                out.write(str.c_str(), str.size() - 7);
            }
            else
            {
                decimal = convertHexToDec(line);
                str.erase(0,line.size() + 1);
                tmp.append(str.c_str(), str.size());
            }
            req.firstTime = false;
            req.chun++;
        }
        else 
        {
            size_t a;
            char buffer[1024];
            a = recv(fd, buffer, 1023, 0);
            buffer[a] = '\0';
            tmp.append(buffer, a);
            if((tmp.size() > (decimal + 10))
                || ((tmp.size() >= decimal + 5 ) && (*(tmp.end() - 1) == '\n') && (*(tmp.end() - 2) == '\r')))
            {
                size_t distance = tmp.size() - (decimal + 2);
                out.write(tmp.c_str(), decimal);
                out.flush();
                tmp = tmp.substr(decimal + 2, distance);
                std::istringstream f(tmp);
                std::getline(f, line);
                decimal = convertHexToDec(line);
                req.chun++;
                tmp.erase(0,line.size() + 1);
                std::cout << "now decimal = " << decimal << std::endl;
            }
        }
        if (decimal == 0){
            std::cout << "------------------ LAST CHUNKED --------------------------\n";
            req.connexion = true;
            std::ifstream fi("www/server1/suc.html");
            std::stringstream response;
            response << "HTTP/1.1 201 Created\r\n"
                    << "\r\n"
                    << fi.rdbuf();
            fi.close();
            send(fd, response.str().c_str(), response.str().size(),0);
        }
    }
}



int	Response::DELETE(int &fd, Request &req, Conf &server, std::string dpath)
{
    std::string root =  server.locat.find(req.locationPath)->second.root;
    const char* path = root.c_str();
    const char* path2 = dpath.c_str();

    char resolved_path[PATH_MAX];
    std::stringstream ss;
    std::stringstream ss1;
    ss  << realpath(path, resolved_path);
    ss1 << realpath(path2, resolved_path);
    std::string str1;
    std::string str2;
    ss >> str1 ;
    ss1 >> str2;
    if(directoryExists(str2.c_str()))
    str2 += "/";
    str1 += "/";
    std::cout << "here1 : "<< str1  << std::endl;
    std::cout << "here1 : " << str2  <<  std::endl;
        if(str2.find(str1) != 0)   
            throw forbidden();
    if(directoryExists(str2.c_str()))
    {
        DIR* dir = opendir(str2.c_str());
        if (dir != NULL) 
        {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) 
            {
                std::cout << " ----------   is dir  -----------\n" << entry->d_name <<std::endl;
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                {
                    std::string filePath = str2 + entry->d_name;
                    if(directoryExists(filePath.c_str()))
                        filePath += "/";
                    std::cout << "DELETE : " << filePath <<std::endl;
                    DELETE(fd, req, server, filePath);
                }
            }
            if(req.get_path() != str2)
            {
                std::cout << "here" << str2 <<std::endl;
                if(str2 != server.locat.find(req.locationPath)->second.root)
                    rmdir(str2.c_str());

            }
            req.connexion = true;
            closedir(dir);
        }
    }
    else if(fileExists(str2.c_str()))
    {

        if (access(str2.c_str(), W_OK) != 0)
            throw (forbidden());
        return remove(str2.c_str());
    }
    else
    {
        throw (notFound());
    }
    return 0;
}
