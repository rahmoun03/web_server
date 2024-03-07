

#include "Response.hpp"


size_t  hexadecimal(const std::string& chunkHeader) {
    std::istringstream iss(chunkHeader);
    size_t chunkSize;
    iss >> std::hex >> chunkSize;
    return chunkSize;
}



void    Response::generateResponse(int &fd, Request &req)
{
    if (req.get_method() == "GET")
    {
        std::cout << RED << "GET METHOD" << DEF << std::endl;

        if(((extension(req.get_path())) == "jpeg") || ((extension(req.get_path())) == "png"))
            imageFile(fd, req);
        else if (req.get_path() == SERVER_ROOT)
            throw "bad request 400";
        else
            htmlFile(fd, req);
    }
    else if(req.get_method() == "POST")
    {
        // size_t chunked_size;
        // size_t chunked_count;
        //   std::string chunked;
        int flag = 0;
		        // static std::string chunked;
        if (req.firstTime)
        {
            std::cout << "-----POST START HERE && CREATE FILE AND NAME-----\n";
            std::string type = static_cast<std::string>(req.get_header("Content-Type:"));
            type.erase(std::remove_if(type.begin(),type.end(),isspace),type.end());
            type.erase(type.find("/"));
            type.push_back('.');
            std::string path = ("./assets/upload/"+ type + req.get_header("Content-Type:").substr(req.get_header("Content-Type:").find("/") + 1));
            std::cout << RED << path << DEF << std::endl;
            // std::cout << "-----HERE-----\n";
            out.open(path.c_str());
                std::cout << "----------------is fill---------------------\n";
            if (!req.get_body().empty())
            {
                if (!req.get_header("Transfer-Encoding:").empty()){
                    std::cout << "------------INSIDE GETBODY-------------\n";
                    chunked = req.get_body();
                    flag = 1;
                }
                else{
                    out.write(req.get_body().c_str(),req.get_body().size());
                }
                out.flush();
            }
            req.firstTime = false;
        }
        // if (!req.get_header("Transfer-Encoding:").empty()){
        //         // std::string Data;
        //         // std::string tmp;
        //         std::string chunkHeader;
		//         // static std::string chunked;
        //         // std::stringstream 
        //         std::vector<std::string> data;
        //         static size_t chunkSize;
        //         // std::string chunked;
        //         size_t pos;
        //         // size_t ins;
        //         if (flag){
        //             std::cout << "------FRIST TIME HERE ---------\n";
        //                 //  Data = chunked;
        //                 if ((pos = chunked.find("\r\n")) != std::string::npos){
        //                     chunkHeader = chunked.substr(0, pos);
        //                     chunkSize = hexadecimal(chunkHeader);
        //                     chunked.erase(0, pos + 2);
        //                     std::cout << "size of data  ---- \n" << chunked.size() << ": <> : "<< chunkSize << std::endl;
        //                 }
        //                 // std::cout << "size of chunked befor \n ====> : " << chunkSize << std::endl;
        //                 size_t os;
        //                 if ((os = chunked.find("\r\n")) != std::string::npos){
        //                     std::cout << chunked.substr(os,os + 2) << std::endl;
        //                     std::cout << "0-------------0\n";
        //                 }

        //                 tmp.append(chunked,chunked.size());
        //                 // out.write(Data.c_str(),Data.size());
        //                 // out.flush();

        //         }
        //         exit(0);
        //         // ssize_t a = -1;
        //         // char buffer[1024];
        //         // if ((a = recv(fd, buffer, 1023, 0)) == -1)
        //         // {
        //         //     std::cerr << "failure in read request !" << std::endl;
        //         //     exit(1);
        //         // }
        //         // buffer[a] = '\0';
        //         // tmp.append(buffer,a);
        //         // if (tmp.size() > chunkSize){
        //         //     std::cout << "size of tmp && chunked size is \n --- :> " << tmp.size() << "-|-" << chunkSize << std::endl;
        //         //     // pos = tmp.find("\r\n");
        //         //     if ((pos = tmp.find("\r\n")) != std::string::npos){
        //         //         std::string ss = tmp.substr(pos - 2, pos);
        //         //         // size_t s = ss.find("\r\n");
        //         //         std::cout << "----------TMP BODY IS ----------\n :> " << ss << std::endl;
        //         //     //     // if (s != std::string::npos){
        //         //     //     // }
        //         //     }
        //         //     exit(0);
        //         // }
        //         // out.write(tmp.c_str(),tmp.size());
        //         // out.flush();

        // }
        if (!req.get_header("Content-Length:").empty() && !(atoi(req.get_header("Content-Length:").c_str()) == (int)req.get_body().size())){
                // std::cout << "-----------HERE-----------\n";
                ssize_t a = -1;
                char buffer[1024];
                if ((a = recv(fd, buffer, 1023, 0)) == -1)
                {
                    std::cerr << "failure in read request !" << std::endl;
                    exit(1);
                }
                buffer[a] = '\0';
                out.write(buffer, a);
                out.flush();
                if (static_cast<int>(out.tellp()) < (int)req.get_body().size()){
                    out.close();
                    req.connexion = true;
                }
                
            }
    }
    else if(req.get_method() == "DELETE")
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

void	Response::htmlFile(int &fd, Request &req)
{
    if (req.get_path() == ("./assets/"))
    {
        std::cout << " <  ---------- home --------.\n" << std::endl;
        std::string content = homepage();
        std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        send(fd, content.c_str(), content.size(), 0);
    }
    else
    {
        std::ifstream file(req.get_path().c_str());
        if(!file.is_open())
        {
            std::cout << " <  ---------- Error file --------.\n" << std::endl;
            throw (notFound());
            // std::string content = ;
            // std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
            // send(fd, content.c_str(), content.size(), 0);
        }
        else
        {
            std::cout << " <  ---------- YES --------.\n" << std::endl;
            std::string content = getResource(file, "text/", extension(req.get_path()));
            std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
            send(fd, content.c_str(), content.size(), 0);
        }
    }
}
void	Response::imageFile(int &fd, Request &req)
{
    std::ifstream img(req.get_path().c_str());
    if(!img.is_open())
    {
        std::cout << " <  ---------- Error image --------.\n" << std::endl;
        throw( notFound());
        // std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
        // send(fd, content.c_str(), content.size(), 0);
    }
    else
    {
        std::string content = getImage(img, "image/", extension(req.get_path()));
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

