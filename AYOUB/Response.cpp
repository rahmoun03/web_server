

#include "Response.hpp"
#include "Request.hpp"
#include "../CHEBCHOUB/conf.hpp"


#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>   //close
#include <ctime>
#include <sys/stat.h>
#include <sys/wait.h>



void Response::generateResponse(int &fd, Request &req, Conf &server , epoll_event &event)
{
    if(req.firstTime)
    {
        checkHeaders(req, server);
        std::string _path = req.get_path();
        while (server.locat.find(_path + "/") == server.locat.end())
        {
            size_t e = _path.rfind("/");
            _path = _path.substr(0, e);
        }
        if (server.locat.find(_path + "/") != server.locat.end())
        {
            loca _location = server.locat.find(_path + "/")->second;
            // std::cout << "location : " << (_path + "/") << std::endl;
            // std::cout << "Add root : " << (_location.root) << std::endl;
            req.root_end = strlen((_location.root).c_str());
            req.get_path() = _location.root + req.get_path();
            req.red_path = _location.redirect;
            req.locationPath = (_path + "/");
        }
        if((req.get_method() == "GET" && !(server.locat.find(req.locationPath)->second.get))
            || (req.get_method() == "DELETE" && !(server.locat.find(req.locationPath)->second.delet))
            || (req.get_method() == "POST" && !(server.locat.find(req.locationPath)->second.post)))
        {
            throw (notAllow(req.get_method(), server.confCherch("405"), req));
        }
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
            //           << server.locat.find(req.locationPath)->second.upload << std::endl; 
            POST(fd, req, server, event);
        }
        else
        {
            std::cout << "dont suport upload" << std::endl;
            // if(!postToGet)
            // event.events = EPOLLOUT;
            // // postToGet = true;
            // req.query = req.get_body();
            // GET(fd, req, server);

            throw(notImplement(server.confCherch("501"), req));
        }
    }

    else if (req.get_method() == "DELETE")
    {
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
            // std::cout << YOLLOW << "send response to client ==> " << DEF << std::endl;
            if(send(fd, response.str().c_str(), response.str().size(), 0) == -1)
            {
                perror("send :");
                throw serverError(server.confCherch("500"),req);
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
            // std::cout << YOLLOW << "send response to client ==> " << DEF << std::endl;
            if (send(fd, response.str().c_str(), response.str().size(), 0) == -1)
                throw true;
            req.connexion = true;
        }
    }

    else
    {
        req.connexion = true;
        std::cout << RED << "UNKOWN METHOD" << DEF << std::endl;
    }
}

void Response::serv_file(map_iterator &type, int &fd, Request &req, Conf &server)
{

    if (req.firstTime)
    {
        file = open(req.get_path().c_str(), O_RDONLY);
        // std::cout << "first time , fd file = " << file << std::endl;
        if (file < 0)
        {
            std::cout << " <  ---------- Error file --------->\n"
                      << std::endl;
            throw(notFound(server.confCherch("404"), req));
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
                //   << YOLLOW << response.str() << std::endl;
        // std::cout << YOLLOW << "send response to client" << DEF << std::endl;
        if (send(fd, response.str().c_str(), response.str().size(), 0) ==-1)
            throw true;
        req.firstTime = false;
    }
    else
    {
        // std::cout << "second time" << std::endl;
        std::string content = getResource(file, req, server);
        // std::cout << RAN << content << DEF << std::endl;
        std::cout << YOLLOW << "send response to client wowowowoow" << DEF << std::endl;
        if (send(fd, content.c_str(), content.size(), 0) == -1)
            throw true;
        // req.connexion = true;
    }
}

void Response::serv_dir(int &fd, Request &req, Conf &server)
{
    std::string _path = (req.get_path());
    if (_path == SERVER_ROOT)
        throw(badRequest(server.confCherch("400"), req));

    else
    {
        if (*(_path.end() - 1) == '/')
        {
            // std::cout << GREEN <<"location : " << req.locationPath <<DEF << std::endl;
            if (server.locat.find(req.locationPath) != server.locat.end()
                && server.locat.find(req.locationPath)->second.autoindex)
            {
                // std::cout << "the location : " << _path.substr(req.root_end) << "\nautoIndex : "
                        //   << (server.locat.find(_path.substr(req.root_end))->second.autoindex) << std::endl;
                std::cout << BLUE << "Listing The Directory ..." << DEF << std::endl;

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
                if (send(fd, response.str().c_str(), response.str().size(), 0) == -1)
                    throw true;
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
                    serv_file(it, fd, req, server);
                }
                else if (server.locat.find(req.locationPath)->second.cgi && (extension(_path) == "php" || extension(_path) == "py"))
                {
                    std::cout << "http://" << _path << "\n";
                    std::cout << "the index is a file : php" << std::endl;

                    req.get_path() = _path;
                    if(!serveCgi(req,fd))
                    {
                        if (cgirespons)
                        {
                            fclose(output_file);
                            std::ifstream ff(temp_file.c_str());
                            std::stringstream response;
                            std::string res = std::string(std::istreambuf_iterator<char>(ff), std::istreambuf_iterator<char>()); 
                            response << "HTTP/1.1 200 OK\r\n"
                                    << res;
                            printf("%s\n", response.str().c_str());
                            if (send(fd, response.str().c_str() , response.str().size(), 0) == -1)
                                throw true;
                            req.connexion = true;
                            ff.close();
                        }
                    }
                    else
                    {
                        std::cout << "CGI ERROR\n";
                        throw (serverError(server.confCherch("500"), req));
                    }
                    req.firstTime = false;
                }
                else
                {
                    // std::cout << "this if forbidden folder" << std::endl;
                    throw(forbidden(server.confCherch("403"), req));
                }
            }
        }
        else
        {
            std::string location = (_path.substr(req.root_end)) + "/";

            std::cout << "make a redirection URL " << std::endl;
            std::cout << "from this : " << _path.substr(req.root_end) << std::endl;
            std::cout << "to   this : " << location << std::endl;

            Redirect(location, req, fd, server);
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
        throw(notImplement(server.confCherch("501"), req));
    }
    if (req.get_method() == "POST" && !head.count("Content-Type:"))
    {
        std::cout << "Content-Type Not exist" << std::endl;
        throw(notImplement(server.confCherch("501"), req));
    }
    if((req.get_method() != "GET") && (req.get_method() != "POST") && (req.get_method() != "DELETE"))
    {
        std::cout << "requist line not correct Method " << std::endl;
        throw (badRequest(server.confCherch("400"), req));
    }
    if(req.get_method().empty() || req.get_path().empty() || !req.startLineForma)
    {
        std::cout << "requist line not correct "<< (req.startLineForma ? "yes" : "no") << std::endl;
        throw(badRequest(server.confCherch("400"), req));
    }
    if(req.get_path().empty() || req.get_path()[0] != '/')
    {
        std::cout << "requist line not correct ohhhh "<< std::endl;
        std::cout << req.get_path()<< std::endl;
        throw(badRequest(server.confCherch("400"), req));
    }
    // {
    //     std::cout << "requist line not correct "<< (req.startLineForma ? "yes" : "no") << std::endl;
    //     throw(badRequest(server.confCherch("400"), req));
    // }
    if (req.get_method() == "POST" && !head.count("Transfer-Encoding:") && !head.count("Content-Length:"))
    {
        std::cout << "TE and CL Not exist" << std::endl;
        throw(lengthRequired(server.confCherch("411"), req));
    }
    if ((req.get_method() == "GET") && (!req.get_body().empty() || head.count("Content-Length:")))
    {
        std::cout << "find Content-Lenght or Body" << std::endl;
        throw(badRequest(server.confCherch("400"), req));
    }
    if (head.count("Transfer-Encoding:") && head.count("Content-Length:"))
    {
        std::cout << "find TE and CL together" << std::endl;
        throw(badRequest(server.confCherch("400"), req));
    }
    if (req.get_protocol().empty() || req.get_protocol() != "HTTP/1.1")
    {
        std::cout << "protocol is : " << req.get_protocol() << std::endl;
        throw(httpVersion(server.confCherch("505"), req));
    }
    if (req.body_limit < std::atol(req.get_header("Content-Length:").c_str()))
    {
        std::cout << "Entity too large : " << req.body_limit << " < " << std::atol(req.get_header("Content-Length:").c_str()) << std::endl;
        throw(EntityTooLarge(server.confCherch("413"), req));
    }
    if (req.get_path().size() > 2048)
    {
        std::cout << "long URI" << std::endl;
        throw(longRequest(server.confCherch("414"), req));
    }
}

Response::Response()
{
}

Response::~Response()
{
}

std::string Response::getResource(int &file, Request &req, Conf &server)
{
    // std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    (void)server;
    std::stringstream content;
    char buffer[1024];
    size_t a = read(file, buffer, 1023);
    if (a == (size_t)-1)
    {
        std::cout << "file : " << file << std::endl;
        std::cerr << "---read---" << std::endl;
        throw true;
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
        return "null";
    }
    return path.substr(dot + 1);
}

// TODO <=====================              Error pages                 ================================>

// TODO /===================================================================================================/

void Response::clear()
{
    out.close();
    close(file);
    chunked.clear();
    decimal = 0;
    str.clear();
    tmp.clear();
    path.clear();
    file = -1;
    pid = -1;
    firstcgi = false;
    cgirespons = false;
    timeout = false;
    output_file = NULL;
    temp_file.clear();
    firstExcep = false;
    end = 0;
    // start = clock();
    tmp_path.clear();
    // std::cout << RED << "clear response object" << DEF << std::endl;
}

void Response::Redirect(std::string &location, Request &req, int &fd, Conf &server)
{
    (void)server;
    std::string content = getRedirctionS(location);
    // std::cout << BLUE << "respone : \n"
    //           << YOLLOW << content << std::endl;
    std::cout << YOLLOW << "send response to client " << DEF << std::endl;
    send(fd, content.c_str(), content.size(), 0);
        // throw true;
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
             << "        font-family: Arial, sans-serif;"
             << "        justify-content: center;"
             << "        align-items: center;"
             << "        height: 100vh;"
             << "        background-color: #f5f5f5;"
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

std::string random_name()
{
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int len = 5;
    srand(time(0));
    std::string name;
    name.reserve(len);
    for (int i = 0; i < len; i++)
        name += letters[rand() % (sizeof(letters) - 1)];
    return (name + ".txt");
}

int Response::serveCgi(Request &req, int &fd)
{
    std::string php_path = "/usr/bin/php-cgi";
    std::string py_path = "/usr/bin/python3";
    (void)fd;

    const char* args[3];
    char** env = new char*[8];
    if (!firstcgi)
    {
        printf("hererre\n");
        // std::cout <<  req.get_query() << std::endl;
        firstcgi = true;
        start = clock();
        temp_file = "/tmp/cgi_" + random_name();
        output_file = fopen(temp_file.c_str(), "w");
        if (!output_file) {
            std::cerr << "Failed to open temporary file for writing." << std::endl;
            return 1;
        }
                    env[0] = new char [("QUERY_STRING=" + req.get_query()).size() + 1];
                    strcpy((char *)env[0],("QUERY_STRING=" + req.get_query()).c_str());
                    env[1] = new char [("REQUEST_METHOD=" + req.get_method()).size() + 1];
                    strcpy((char *)env[1],("REQUEST_METHOD=" + req.get_method()).c_str());
                    env[2] = new char [("SCRIPT_FILENAME=" + req.get_path()).size() + 1];
                    strcpy((char *)env[2],("SCRIPT_FILENAME=" + req.get_path()).c_str());
                    env[3] = new char [21];
                    strcpy(env[3], "REDIRECT_STATUS=200");
                    if (req.get_method() != "POST"){
                        env[4] = new char [27];
                        strcpy((char *)env[4],("CONTENT_TYPE=\"text/html\""));
                    }
                    else{
                        env[4] = new char [("CONTENT_TYPE=" + req.get_header("Content-Type:")).size() + 1];
                        strcpy((char *)env[4],("CONTENT_TYPE="+ req.get_header("Content-Type:")).c_str());
                    };
                    env[6] = new char [("HTTP_COOKIE=" + req.get_header("Cookie:")).size() + 1];
                    strcpy(env[6],("HTTP_COOKIE=" + req.get_header("Cookie:")).c_str());
                    env[7] = NULL;
        if (extension(req.get_path()) == "php"){
            args[0] = php_path.c_str();
            args[1] = req.get_path().c_str();
            args[2]  = NULL;
            env[5] = new char [("SCRIPT_NAME=" + php_path).size() + 1];
            strcpy((char *)env[5],("SCRIPT_NAME=" + php_path).c_str());
        }
        else if (extension(req.get_path()) == "py"){
            env[5] = new char [("SCRIPT_NAME=" + py_path).size() + 1];
            strcpy((char *)env[5],("SCRIPT_NAME=" + py_path).c_str());
            args[0] = py_path.c_str();
            args[1] = req.get_path().c_str();
            args[2]  = NULL;
        }
            pid = fork();
            if (pid == 0)
            {
            
                if (freopen(temp_file.c_str(), "w", stdout) == NULL) 
                {
                    std::cerr << "Failed to freopen stdout." << std::endl;
                    fclose(output_file);
                    
                    return 1 ;
                }
                if (execve(args[0], (char* const*)args, env) == -1) 
                {
                    std::cerr << "Failed to execute CGI script." << std::endl;
                    fclose(output_file);
                
                    return 1;
                }
                int i = 0;
                while (env[i]){
                    delete[] env[i];
                    i++;
                }
                delete[] env;
            } 
    }
    else if (pid > 0)
    {
        int status;
        int WAIT_PID = waitpid(pid, &status, WNOHANG);
        if (WAIT_PID == -1){
            perror("waitpid");
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            std::cout << "we kill this process : "<< pid << std::endl;
            req.connexion = true;
            return 1;
        }
        else if (WAIT_PID == 0)
        {
            end = (double)(clock() - start) / CLOCKS_PER_SEC;
            if (end >= 5.00){
                timeout = true;
                kill(pid, SIGKILL);
                std::cout << "we kill this process : "<< pid << std::endl;
                waitpid(pid, &status, 0);
                firstcgi = false;
                fclose(output_file);
            }

        }
        else
            cgirespons = true;
    }
    else
    {
        std::cerr << "Fork failed." << std::endl;
        fclose(output_file);
        return 1;
    }
    
    return 0;
}

