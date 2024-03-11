#include "Response.hpp"

void	Response::GET(int &fd, Request &req)
{
    std::map<std::string, std::string> map = ErrorAssets();
    map_iterator it = map.find(req.get_path());

    std::cout << "old URL : " << req.get_path() << std::endl;
    if(it != map.end())
        req.get_path() = it->second;
    else
        req.get_path() = (SERVER_ROOT + req.get_path());
    std::cout << "new URL : " << req.get_path() << std::endl;
    if(directoryExists(req.get_path()))
    {
        std::cout << "http://{" <<req.get_path() << "} \n";
        std::cout << "the URL is a directory \n";
        serv_dir(fd, req);
    }
    else if (fileExists(req.get_path()))
    {
        std::map<std::string , std::string> mime_map = mimeTypes();
        it = mime_map.find(extension(req.get_path()));
        if(it != mime_map.end())
        {
            std::cout << "the URL is a file : " << it->second << std::endl;
            serv_file(it, fd, req);
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

void	Response::POST(int &fd, Request &req){

    (void) fd;
    (void) req;
    //TODO khdam awld l9hba matb9ach tzkak
    // if (blackhole == 20){
        //ayoub.freze();
        //chebchoub.sizeofzeb() == 20cm;
    // }

};

void	Response::DELETE(int &fd, Request &req){
    (void) fd;
    (void) req;
    return;
};