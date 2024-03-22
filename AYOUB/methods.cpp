#include "Response.hpp"
#include <filesystem>
#include<stdio.h>
#include<sys/stat.h>
#include<iostream>
#include<string.h>
#include<string>
#include<dirent.h>

       #include <sys/types.h>
       #include <sys/wait.h>

void Response::serv_cgi(int &fd, Request &req) {
    (void)fd;
    (void)req;
    // int file;
    // int cgi_pid;
    // int pip[2];
    // char* args[3];
    // args[0] = const_cast<char*>(req.get_path().c_str());
    // args[1] = const_cast<char*>("/usr/local/bin/php-cgi");
    // args[2] = NULL;
    // char* env[] = {const_cast<char*>("REQUEST_METHOD=GET"), const_cast<char*>("SCRIPT_FILENAME=/path/to/cgi_script.cgi"), NULL};

    // file = open(req.get_path().c_str(), O_RDONLY);
    std::cout << "content of file" << req.get_path().c_str() << std::endl;
    // if (file < 0) {
    //     exit(0);
    //     // throw (notFound());
    // }
    // char buff[1024];
    // re(file,buff,1023,0);
    // std::cout << "bpdy of file : " << buff << std::endl;
    // exit(0);

    // if (pipe(pip) == -1) {
    //     perror("pipe : ");
    //     exit(0);
    // }

    // cgi_pid = fork();
    // if (cgi_pid == 0) {
    //     // close(pip[1]);
    //     // dup2(pip[0], file);
    //     // dup2(fd, STDOUT_FILENO);
    //     execve(args[0], args, env);
    //     exit(0); 
    // } else if (cgi_pid == -1) {
    //     perror("fork : ");
    //     exit(0);
    // }
    // write()

    // Close unused file descriptors
    // fd = pip[1];
    // close(pip[0]);
    // close(file);
    // const char* http_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    // send(fd, http_header, strlen(http_header), 0);
    // // int status;
    // // waitpid(cgi_pid, &status, 0);
    // req.connexion = true;
    exit(0);
}

void	Response::GET(int &fd, Request &req, Conf &server)
{
    (void) server;
    std::string _path = req.get_path();
    std::map<std::string, std::string> map = ErrorAssets();
    map_iterator it = map.find(req.get_path());
    if(req.firstTime)
    {
        std::cout << "old URL : " << req.get_path() << std::endl;
        if(it != map.end())
            req.get_path() = it->second;
        else
        {
            std::cout << "test : " << _path << std::endl;
            // exit(0);
            while (server.locat.find(_path + "/") == server.locat.end())
            {
                size_t e = _path.rfind("/");
                _path = _path.substr(0, e);
            }
            if (server.locat.find(_path + "/") != server.locat.end())
            {
                std::cout << "Add root to the URI ..." << (server.locat.find(_path + "/")->second.root) << std::endl;
                req.root_end = strlen((server.locat.find(_path + "/")->second.root).c_str());
                req.get_path() = server.locat.find(_path + "/")->second.root + req.get_path();
                req.CGI =  server.locat.find(_path + "/")->second.cgi ;
            }
        }
        std::cout << "new URL : " << req.get_path() << std::endl;
    }
    if(directoryExists(req.get_path()))
    {
        std::cout << "http://{" << req.get_path() << "} \n";
        std::cout << "the URL is a directory \n";
        serv_dir(fd, req, server);
    }
    else if (fileExists(req.get_path()))
    {
        std::map<std::string , std::string> mime_map = mimeTypes();
        it = mime_map.find(extension(req.get_path()));
        if(it != mime_map.end() || extension(req.get_path()) == "cpp")
        {
            std::cout << "http://{" << req.get_path() << "} \n";
            // // if (server.locat.find("/")->second.cgi)
            // std::cout << "---------------------HERE--------------------\n";
            if (!req.CGI)
            {
                std::cout << "the URL is a file : " << it->second << std::endl;
                serv_file(it, fd, req);
            }
            else 
                serv_cgi(fd,req);
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
            std::string up_ptah = server.locat.find(req.get_path())->second.upload;
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
        if (req.firstTime)
        {
            std::cout << "request path : " << req.get_path() << std::endl;
            std::string up_ptah = server.locat.find(req.get_path())->second.upload;
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
            str = req.get_body();
            std::istringstream f(str);
            std::getline(f, line);
            std::cout << line <<std::endl;
            decimal = convertHexToDec(line);
            req.chun++;
            out.open(path.c_str(), std::ios::binary);
            str.erase(0,line.size() + 1);
            tmp.append(str.c_str(), str.size());
            req.firstTime = false;
        }
        else 
        {
            size_t a;
            char buffer[1024];
            a = recv(fd, buffer, 1023, 0);
            buffer[a] = '\0';
            tmp.append(buffer, a);
            if(tmp.size() >= decimal)
            {
                size_t distance = tmp.size() - decimal;
                out.write(tmp.c_str(), decimal);
                out.flush();
                tmp = tmp.substr(decimal + 2, distance);
                std::istringstream f(tmp);
                std::getline(f, line);
                decimal = convertHexToDec(line);
                req.chun++;
                tmp.erase(0,line.size() + 1);
            }
        }
        if (decimal == 0){
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

void	Response::DELETE(int &fd, Request &req, Conf &server)
{
    (void) server;
    std::map<std::string, std::string> map = ErrorAssets();
    map_iterator it = map.find(req.get_path());
    if(req.firstTime)
    {

        std::cout << "old URL : " << req.get_path() << std::endl;
        if(it != map.end())
            req.get_path() = it->second;
        else
            req.get_path() = (SERVER_ROOT + req.get_path());
        std::cout << "new URL : " << req.get_path() << std::endl;
    }
    std::cout << "you want to delete : " << req.get_path() << std::endl;
    // exit(0);
    DIR* dir = opendir(req.get_path().c_str());
    if (dir != NULL) 
    {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) 
        {
        std::cout << " ----------   is dir  -----------\n" << entry->d_name <<std::endl;
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
            {
                req.get_path() = req.get_path() + "/" + entry->d_name;
                struct stat st;
                if (lstat(req.get_path().c_str(), &st) == 0) 
                {
                    if (S_ISDIR(st.st_mode))
                        DELETE(fd, req, server); 
                }
            }
        }
            req.connexion = true;
        closedir(dir);
    }
    else if(fileExists(req.get_path()))
    {
        if (remove(req.get_path().c_str()) != 0)
        {
            std::stringstream response;
            response << "HTTP/1.1 204 No Content\r\n"
             << "Connection: close\rEncoding\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "\r\n";
            
            send(fd, response.str().c_str(), response.str().size(), 0);
            req.connexion = true;
        }
        else
        {
            std::ifstream ff("www/server1/delete.html", std::ios::binary);
            
            std::stringstream response;
            response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/html\r\n"
             
             << "Connection: close\rEncoding\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "\r\n"
             << ff.rdbuf();
            
            std::cout << "response :\n" << response.str()<<std::endl;

            if(send(fd, response.str().c_str(), response.str().size(), 0) == -1)
            {
                perror("send :");
                exit(1);
            }
            req.connexion = true;
            ff.close();
            // std::cout << "the file was deleted ..." << std::endl;
        }
    }
    else
    {
        throw (notFound());
    }    
    return;
}
