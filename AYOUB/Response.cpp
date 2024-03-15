

#include "Response.hpp"

void    Response::generateResponse(int &fd, Request &req)
{
    checkHeaders(req);
    // std::string
    if (req.get_method() == "GET")
    {
        std::cout << RED << "GET METHOD" << DEF << std::endl;
        GET(fd, req);
    }

    else if(req.get_method() == "POST")
    {
        std::cout << RED << "POST METHOD" << DEF << std::endl;
        if(SUPORT_UPLOAD)
            POST(fd, req);
        else
        {
            std::cout << "dont suport upload" << std::endl;
            GET(fd, req);
        }
    }

    else if(req.get_method() == "DELETE")
    {
        std::cout << RED << "DELETE METHOD" << DEF << std::endl;
        DELETE(fd, req);
    }

    else
    {
        std::cout << RED << "UNKOWN METHOD" << DEF << std::endl;
    }
}

void Response::serv_file(map_iterator &type, int &fd, Request &req)
{
    if(req.firstTime)
    {
        file = open(req.get_path().c_str(), O_RDONLY);
        std::cout << "first time , fd file = "<< file << std::endl;
        if(file < 0)
        {
            std::cout << " <  ---------- Error file --------->\n" << std::endl;
            throw (notFound());
        }
        std::cout << " <  ---------- YES --------->\n" << std::endl;
        std::stringstream response;
        response << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: " << type->second << "\r\n"
        // << "Content-Length: "<< buffer.size() <<"\r\n"
        << "Connection: keep-alive\r\n"
        << "Server: " << "chabchoub" << "\r\n"
        << "Date: " << getCurrentDateTime() << "\r\n"
        << "\r\n";


        std::cout<< BLUE<<"respone : \n"<<YOLLOW<< response.str()  << std::endl;
        send(fd, response.str().c_str(), response.str().size(),0);
        req.firstTime = false;
    }
    else
    {
        std::cout << "second time" << std::endl;
        std::string content = getResource(file, req);
        std::cout << RAN << content << DEF << std::endl;
        send(fd, content.c_str(), content.size(),0);
        // req.connexion = true;
    }
}

void	Response::serv_dir(int &fd, Request &req)
{
    std::string path = (req.get_path());
    if ( path == SERVER_ROOT)
        throw (badRequest());
    // if ( path == "./www/server1/")
    // {
    //     std::cout << " <  ---------- home --------->\n" << std::endl;
    //     std::string content = homepage();
    //     std::cout<< BLUE<<"respone : \n"<<YOLLOW<<content  << std::endl;
    //     send(fd, content.c_str(), content.size(),0);
    //     req.connexion = true;
    // }
    else
    {
        if(req.firstTime)
        {
            std::string path = req.get_path();
            if(*(path.end()-1) == '/')
            {
                path += "index.html";
                // std::cout << "open index file "<< std::endl;
                std::map<std::string , std::string> mime_map = mimeTypes();
                map_iterator it = mime_map.find(extension(path));
                if(it != mime_map.end() && fileExists(path))
                {
                    file = open(path.c_str(), O_RDONLY);
                    // std::cout << "the URL is a file : " << it->second << std::endl;
                    // std::cout << " <  ---------- YES --------->\n" << std::endl;
                    std::stringstream response;
                    response << "HTTP/1.1 200 OK\r\n"
                                << "Content-Type: " << it->second << "\r\n"
                                // << "Content-Length: "<< buffer.size() <<"\r\n"
                                << "Connection: close\r\n"
                                << "Server: " << "chabchoub" << "\r\n"
                                << "Date: " << getCurrentDateTime() << "\r\n"
                                << "\r\n";


                    // std::cout<< BLUE<<"respone : \n"<<YOLLOW<< response.str()  << std::endl;
                    send(fd, response.str().c_str(), response.str().size(),0);
                    req.firstTime = false;

                }
                else
                {
                    // this is a forbidden folder
                    std::cout << "this if forbidden folder"<< std::endl;
                    throw(forbidden());
                }
            }
            else
            {
                std::string locaition = (path.substr(path.rfind("/"))) + "/";
                path +=  "/index.html";
                std::cout << "make a redirection URL "<< std::endl;
                std::map<std::string , std::string> mime_map = mimeTypes();
                map_iterator it = mime_map.find(extension(path));
                if(it != mime_map.end() && fileExists(path))
                {
                    std::ifstream file(path.c_str());
                    std::cout << "the URL is a file : " << it->second << std::endl;
                    std::cout << " <  ---------- YES --------->\n" << std::endl;
                    std::string content = getRedirctionS(it->second, locaition);
                    std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
                    send(fd, content.c_str(), content.size(), 0);
                    req.connexion = true;
                }
                else
                {
                    // this is a forbidden folder
                    std::cout << "this if forbidden folder"<< std::endl;
                    throw(forbidden());
                }
            }
        }
        else
        {
            std::string content = getResource(file, req);
            // std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
            send(fd, content.c_str(), content.size(), 0);
            // req.connexion = true;
        }
    }
}

void	Response::checkHeaders(Request &req)
{
    std::map<std::string, std::string> head;

    head = req.get_headers();
    if(head.count("Transfer-Encoding:") && req.get_header("Transfer-Encoding:") != "chunked")
    {
        std::cout << "Transfer-Encoding Not chanked" << std::endl;
        throw(notImplement());
    }
    if(req.get_method() == "POST" && !head.count("Transfer-Encoding:") && !head.count("Content-Length:"))
    {
        std::cout << "TE and CL Not exist" << std::endl;
        throw(badRequest());
    }
    if((req.get_method() == "GET") && ( !req.get_body().empty() || head.count("Content-Length:")))
    {
        std::cout << "find Content-Lenght or Body" << std::endl;
        throw(badRequest());
    }
    if(head.count("Transfer-Encoding:") && head.count("Content-Length:"))
    {
        std::cout << "find TE and CL together" << std::endl;
        throw(badRequest());
    }
    if(req.get_protocol().empty() || req.get_protocol() != "HTTP/1.1")
    {
        std::cout << "protocol is : "<< req.get_protocol() << std::endl;
        throw(httpVersion());
    }
    if( req.body_limit < std::atol(req.get_header("Content-Length:").c_str()))
    {
        std::cout << "Entity too large : "<< req.body_limit << " < " << std::atol(req.get_header("Content-Length:").c_str()) << std::endl;
        throw(EntityTooLarge());
    }
    if(req.get_path().size() > 2048)
    {
        std::cout << "" << std::endl;
        throw (longRequest());
    }
}


Response::Response()
{
}

Response::~Response()
{
}


std::string Response::getResource(int &file, Request &req)
{
    // std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream content;
    char buffer[1024];
    size_t a = read(file, buffer, 1023);
    if(a < 0)
    {
        std::cerr << "l9wada" << std::endl;
        exit(1);
    }
    if(a < 1023)
        req.connexion = true;
    content.write(buffer, a);
    return content.str();
}

std::string Response::getRedirctionS(std::string &type, std::string &location)
{
    std::stringstream response;
    response << "HTTP/1.1 301 Moved Permanently\r\n"
            << "Location: " << location << "\r\n"
            << "Content-Type: "<< type << "\r\n"
            << "Connection: close\r\n"
            << "Server: " << "chabchoub" << "\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n"
            << "\r\n";
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
             << "Connection: close\rEncoding\n"
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




void Response::clear()
{
	out.close();
    close(file);
	chunked.clear();
    decimal = 0;
    str.clear();
	tmp.clear();
    file = -1;
}


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

size_t  hexadecimal(const std::string& chunkHeader) {
    std::istringstream iss(chunkHeader);
    size_t chunkSize;
    iss >> std::hex >> chunkSize;
    return chunkSize;
}