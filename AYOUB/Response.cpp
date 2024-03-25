

#include "Response.hpp"

void Response::generateResponse(int &fd, Request &req, uint32_t &event, Conf &server)
{
    (void)event;
    if(req.firstTime)
    {
        std::string _path = req.get_path();
        while (server.locat.find(_path + "/") == server.locat.end())
        {
            size_t e = _path.rfind("/");
            _path = _path.substr(0, e);
        }
        if (server.locat.find(_path + "/") != server.locat.end())
        {
            loca _location = server.locat.find(_path + "/")->second;
            std::cout << "location : " << (_path + "/") << std::endl;
            std::cout << "Add root : " << (_location.root) << std::endl;
            req.root_end = strlen((_location.root).c_str());
            req.get_path() = _location.root + req.get_path();
            req.red_path = _location.redirect;
            req.locationPath = (_path + "/");
        }
        checkHeaders(req, server);
    }
    if (req.get_method() == "GET")
    {
        // std::cout << RED << "GET METHOD for " << DEF << server.confCherch("server_name") << std::endl;
        GET(fd, req, server);
    }
    else if (req.get_method() == "POST")
    {
        if (server.locat.find(req.locationPath) != server.locat.end() && !(server.locat.find(req.locationPath)->second.upload.empty()))
        {
            // std::cout << RED << "POST METHOD, upload path : " << DEF
                    //   << server.locat.find(req.get_path())->second.upload << std::endl;
            POST(fd, req, server);
        }
        else
        {
            // std::cout << "dont suport upload" << std::endl;
            GET(fd, req, server);
        }
    }

    else if (req.get_method() == "DELETE")
    {
        // std::map<std::string, std::string> map = ErrorAssets();
        // map_iterator it = map.find(req.get_path());
        // if(req.firstTime)
        // {

        //     // std::cout << "old URL : " << req.get_path() << std::endl;
        //     if(it != map.end())
        //         req.get_path() = it->second;
        //     else
        //         req.get_path() = (SERVER_ROOT + req.get_path());
        //     // std::cout << "new URL : " << req.get_path() << std::endl;
        // }
        // std::cout << RED << "DELETE METHOD" << DEF << std::endl;
        int d = DELETE(fd, req, server, req.get_path());
        if(d == 1)
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
            
            // std::cout << "response :\n" << YOLLOW << response.str() << DEF <<std::endl;
            std::cout << YOLLOW << "send response to client ==> " << DEF << std::endl;
            if(send(fd, response.str().c_str(), response.str().size(), 0) == -1)
            {
                perror("send :");
                exit(1);
            }
            req.connexion = true;
            ff.close();
        }
        else
        {
            std::stringstream response;
            response << "HTTP/1.1 204 No Content\r\n"
             << "Connection: close\r\n"
             << "Server: " << "chabchoub" << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "\r\n";
            
            // std::cout << "response :\n" << YOLLOW << response.str() << DEF <<std::endl;
            std::cout << YOLLOW << "send response to client ==> " << DEF << std::endl;
            send(fd, response.str().c_str(), response.str().size(), 0);

            
            req.connexion = true;
        }
    }

    else
    {
        req.connexion = true;
        std::cout << RED << "UNKOWN METHOD" << DEF << std::endl;
    }
}

void Response::serv_file(map_iterator &type, int &fd, Request &req)
{

    if (req.firstTime)
    {
        file = open(req.get_path().c_str(), O_RDONLY);
        // std::cout << "first time , fd file = " << file << std::endl;
        if (file < 0)
        {
            std::cout << " <  ---------- Error file --------->\n"
                      << std::endl;
            throw(notFound());
        }
        std::ifstream ff(req.get_path().c_str(), std::ios::binary);

        ff.seekg(0, std::ios::end);
        std::streampos size = ff.tellg();
        ff.seekg(0, std::ios::beg);
        ff.close();

        std::stringstream response;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: " << type->second << "\r\n"
                 << "Content-Length: " << size << "\r\n"
                 << "Connection: keep-alive\r\n"
                 << "Server: "
                 << "chabchoub"
                 << "\r\n"
                 << "Date: " << getCurrentDateTime() << "\r\n"
                 << "\r\n";

        // std::cout << BLUE << "respone : \n"
        //           << YOLLOW << response.str() << std::endl;
        std::cout << YOLLOW << "send response to client" << DEF << std::endl;
        send(fd, response.str().c_str(), response.str().size(), 0);
        req.firstTime = false;
    }
    else
    {
        // std::cout << "second time" << std::endl;
        std::string content = getResource(file, req);
        // std::cout << RAN << content << DEF << std::endl;
        std::cout << YOLLOW << "send response to client " << DEF << std::endl;
        send(fd, content.c_str(), content.size(), 0);
        // req.connexion = true;
    }
}

void Response::serv_dir(int &fd, Request &req, Conf &server)
{
    std::string _path = (req.get_path());
    if (_path == SERVER_ROOT)
        throw(badRequest());

    else
    {
        if (*(_path.end() - 1) == '/')
        {
            std::cout << GREEN <<"location : " << req.locationPath <<DEF << std::endl;
            if (server.locat.find(req.locationPath) != server.locat.end()
                && server.locat.find(req.locationPath)->second.autoindex)
            {
                // std::cout << "the location : " << _path.substr(req.root_end) << "\nautoIndex : "
                        //   << (server.locat.find(_path.substr(req.root_end))->second.autoindex) << std::endl;
                // std::cout << BLUE << "Listing The Directory ..." << DEF << std::endl;

                std::string content = listDirectory(_path.c_str());
                std::stringstream response;
                response << "HTTP/1.1 200 OK\r\n"
                         << "Content-Type: text/html\r\n"
                         << "Content-Length: " << content.size() << "\r\n"
                         << "Connection: close\r\n"
                         << "Server: chabchoub\r\n"
                         << "Date: " << getCurrentDateTime() << "\r\n"
                         << "\r\n"
                         << content;
                std::cout << YOLLOW << "send response to client " << DEF << std::endl;

                send(fd, response.str().c_str(), response.str().size(), 0);
                req.connexion = true;
                req.firstTime = false;
            }
            else
            {
                _path += server.locat.find(req.locationPath)->second.defau;
                std::cout << "open index file :  " << path << std::endl;
                std::map<std::string, std::string> mime_map = mimeTypes();
                map_iterator it = mime_map.find(extension(_path));
                if (it != mime_map.end() && fileExists(_path))
                {
                    req.get_path() = _path;
                    serv_file(it, fd, req);
                }
                else
                {
                    std::cout << "this if forbidden folder" << std::endl;
                    throw(forbidden());
                }
            }
        }
        else
        {
            std::string location = (_path.substr(req.root_end)) + "/";

            // std::cout << "make a redirection URL " << std::endl;
            // std::cout << "from this : " << _path.substr(req.root_end) << std::endl;
            // std::cout << "to   this : " << location << std::endl;

            Redirect(location, req, fd);
        }
    }
}

void Response::checkHeaders(Request &req, Conf &server)
{
    std::map<std::string, std::string> head;
    std::cout << "firts time  : " << req.firstTime << std::endl;
    head = req.get_headers();
    if (head.count("Transfer-Encoding:") && req.get_header("Transfer-Encoding:") != "chunked")
    {
        std::cout << "Transfer-Encoding Not chanked" << std::endl;
        throw(notImplement());
    }
    if(req.get_method().empty() || req.get_path().empty())
    {

        std::cout << "requist line not correct "<< (req.startLineForma ? "yes" : "no") << std::endl;
        throw(badRequest());
    }
    if((req.get_method() == "GET" && !(server.locat.find(req.locationPath)->second.get))
        || (req.get_method() == "DELETE" && !(server.locat.find(req.locationPath)->second.delet))
         || (req.get_method() == "POST" && !(server.locat.find(req.locationPath)->second.post)))
    {
        throw (notAllow(req.get_method()));
    }
    if (req.get_method() == "POST" && !head.count("Transfer-Encoding:") && !head.count("Content-Length:"))
    {
        std::cout << "TE and CL Not exist" << std::endl;
        throw(badRequest());
    }
    if ((req.get_method() == "GET") && (!req.get_body().empty() || head.count("Content-Length:")))
    {
        std::cout << "find Content-Lenght or Body" << std::endl;
        throw(badRequest());
    }
    if (head.count("Transfer-Encoding:") && head.count("Content-Length:"))
    {
        std::cout << "find TE and CL together" << std::endl;
        throw(badRequest());
    }
    if (req.get_protocol().empty() || req.get_protocol() != "HTTP/1.1")
    {
        std::cout << "protocol is : " << req.get_protocol() << std::endl;
        throw(httpVersion());
    }
    if (req.body_limit < std::atol(req.get_header("Content-Length:").c_str()))
    {
        std::cout << "Entity too large : " << req.body_limit << " < " << std::atol(req.get_header("Content-Length:").c_str()) << std::endl;
        throw(EntityTooLarge());
    }
    if (req.get_path().size() > 2048)
    {
        std::cout << "" << std::endl;
        throw(longRequest());
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
    if (a < 0)
    {
        std::cerr << "l9wada" << std::endl;
        exit(1);
    }
    if (a < 1023)
        req.connexion = true;
    content.write(buffer, a);
    return content.str();
}

std::string Response::getRedirctionS(std::string &location)
{
    std::stringstream response;
    response << "HTTP/1.1 301 Moved Permanently\r\n"
             << "Location: " << location << "\r\n"
             << "Server: chabchoub\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n"
             << "\r\n";
    return response.str();
}


std::string Response::extension(const std::string &path)
{
    size_t dot = path.rfind('.');
    if (dot == std::string::npos)
    {
        std::cerr << RED << "failure to find extension !" << DEF << std::endl;
        return "";
    }
    return path.substr(dot + 1);
}

// TODO <=====================              Error pages                 ================================>
// std::string Response::homepage()
// {
//     std::ifstream fi((SERVER_ROOT + std::string("/index.html")).c_str());
//     if (!fi.is_open())
//     {
//         std::cerr << RED << "failure in home page" << std::endl;
//         throw(notFound());
//     }
//     std::cout << "the file is excite \n"
//               << std::endl;
//     std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
//     std::stringstream response;
//     response << "HTTP/1.1 200 OK\r\n"
//              << "Content-Type: text/html\r\n"
//              << "Connection: close\rEncoding\n"
//              << "Server: "
//              << "chabchoub"
//              << "\r\n"
//              << "Date: " << getCurrentDateTime() << "\r\n"
//              << "Content-Length: " << buffer.size() << "\r\n"
//              << "\r\n"
//              << buffer.c_str();
//     return response.str();
// }
// TODO /===================================================================================================/

void Response::clear()
{
    out.close();
    close(file);
    chunked.clear();
    decimal = 0;
    str.clear();
    tmp.clear();
    file = -1;
    // std::cout << RED << "clear response object" << DEF << std::endl;
}

void Response::Redirect(std::string &location, Request &req, int &fd)
{
    std::string content = getRedirctionS(location);
    // std::cout << BLUE << "respone : \n"
    //           << YOLLOW << content << std::endl;
    std::cout << YOLLOW << "send response to client " << DEF << std::endl;
    send(fd, content.c_str(), content.size(), 0);
    req.connexion = true;
    req.firstTime = false;
}

#include <dirent.h>
std::string listDirectory(const char *path)
{
    std::stringstream response;
    response << "<html><head><title>Directory Listing</title>"
             << "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/icon?family=Material+Icons\">"
             << "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@24,400,0,0\" />"
             << "<style>"
             << ".li , a{ display : flex; gap : 4px;align-items : center; width: 30%;} "
             << "a:hover {background-color: #00000059; border-radius : 2px; }"
             << "a {color: black; font-size: 14px;}"
             << "li, a { "
             << "text-decoration : none;"
             << "list-style-type: none;"
             << "} *{box-sizing : border-box;}"
             << "    body {"
             << "        display: flex;"
             << "        justify-content: center;"
             << "        align-items: center;"
             << "        height: 100vh;"
             << "    }"
             << "    .main-content {"
             << "        border-radius: 50px;"
             << "        display: block;"
             << "        width: 80%;"
             << "        height: 80%;"
             << "        justify-content: center;"
             << "        align-items: center;"
             << "        text-align: center;"
             << "        padding: 20px;"
             << "    }"
             << "    .main-content .list {"
             << "        list-style-type: none;"
             << "        display : flex; align-items : center; gap : 30px; width : 100%; margin: 30px ; flex-wrap : wrap"
             << "    }"
             << "    .main-content h1 {"
             << "        width: 40%;"
             << "        height: auto;"
             << "        display: block;"
             << "        margin: 0 auto;"
             << "        font-size: 40px;"
             << "        color: black;"
             << "    }"
             << "</style>"
             << "</head>"
             << "<body>"
             << "    <div class=\"main-content\">"
             << "    <h1>Directory Listing</h1>"
             << "    <hr size=\"2\" color=\"black\" width=\"90%\">"
             << "    <div class=\"list\">";

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {

            if ((int)entry->d_type == 4)
            {
                response << "<a href=\"" << entry->d_name << "\"> <li class='li'><i  style='color: #edbe00;' class=\"material-icons\">folder</i>";
            }
            else if ((int)entry->d_type == 8)
            {
                response << "<a href=\"" << entry->d_name << "\"> <li class='li'><i  style='color: #edbe00;'class='material-symbols-outlined'>draft</i>";
            }
            else
            {
                response << "<a href=\"" << entry->d_name << "\"> <li class='li'><i  style='color: #edbe00;'class='material-symbols-outlined'>unknown_document</i>";
            }
            // std::cout << "type : " << (int)entry->d_type << ", directory name :" << entry->d_name << std::endl;
            response << entry->d_name << " </li></a>";
        }
        closedir(dir);
    }
    else
    {
        perror("opendir");
    }
    response << "</div></div>";
    response << "</body></html>";
    // std::cout << "\n\nherrrerrer \n\n"
    //           << response.str() << "\n\n endddddd" << std::endl;
    return response.str();
}

std::string getCurrentDateTime()
{
    char buffer[80];
    std::time_t rawtime;
    std::tm *timeinfo;

    std::time(&rawtime);
    timeinfo = std::gmtime(&rawtime);

    // Format the date and time
    std::strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    return std::string(buffer);
}

bool directoryExists(std::string path)
{
    struct stat info;

    // Use stat to check if the directory exists
    if (stat(path.c_str(), &info) != 0)
    {
        // If stat returns non-zero, the directory doesn't exist
        return false;
    }

    // Check if it is a directory
    return (info.st_mode & S_IFDIR) != 0;
    // S_IFREG
}

bool fileExists(std::string path)
{
    struct stat info;

    // Use stat to check if the directory exists
    if (stat(path.c_str(), &info) != 0)
    {
        // If stat returns non-zero, the directory doesn't exist
        return false;
    }

    // Check if it is a directory
    return (info.st_mode & S_IFREG) != 0;
    // S_IFREG
}

size_t hexadecimal(const std::string &chunkHeader)
{
    std::istringstream iss(chunkHeader);
    size_t chunkSize;
    iss >> std::hex >> chunkSize;
    return chunkSize;
}
