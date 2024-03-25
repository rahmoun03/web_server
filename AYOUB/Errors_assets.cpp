#include "Response.hpp"

/******************************************************************************/

std::string Response::timeOut(std::string path)
{
    std::ifstream fi(path.c_str());
    std::stringstream response;
    response << "HTTP/1.1 408 Request Timeout\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 408 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"
            << "<html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;"
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0; "
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center;" 
            << "            padding: 20px; "
            << "        }"
            << "        .main-content h1 {"
            << "            width: 40%;"
            << "            height: auto;" 
            << "            display: block;" 
            << "            margin: 0 auto;"
            << "            font-size: 40px;"
            << "            color: azure; "
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"main-content\">"
            << "        <h1>408 Request Timeout</h1>"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>chabchob server</p>"
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                << "\r\n"
                << buffer;
    }
    return response.str();
}


std::string Response::notAllow(std::string method)
{
    std::ifstream fi("www/Errors/405.html");
    std::stringstream response;
    response << "HTTP/1.1 405 Method Not Allowed\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 405 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"
            << "<html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;"
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0; "
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center;" 
            << "            padding: 20px; "
            << "        }"
            << "        .main-content h1 {"
            << "            width: 40%;"
            << "            height: auto;" 
            << "            display: block;" 
            << "            margin: 0 auto;"
            << "            font-size: 40px;"
            << "            color: azure; "
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"main-content\">"
            << "        <h1>405 Method Not Allowed</h1>"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>the method " << method << " Not Allowed, chabchob server</p>"
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                << "\r\n"
                << buffer;
    }
    return response.str();
}


std::string Response::notFound()
{
    srand(time(NULL));
    int f = rand() % 2;
    const char *path[] = {"www/Errors/404.html", "www/Errors/404-2.html"};
    std::stringstream response;
    response << "HTTP/1.1 404 Not Found\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    std::ifstream fi(path[f]);
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 404 page" << std::endl;
        std::stringstream con ;
        con << "<!DOCTYPE html>"
            << "        <html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;" 
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0;" 
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center;" 
            << "            padding: 20px; "
            << "        }"
            << "        .main-content img {"
            << "            width: 40%;"
            << "            height: auto;" 
            << "            display: block;" 
            << "            margin: 0 auto;" 
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "   <div class=\"main-content\">"
            << "        <img src=\"assets/404yellow.svg\" alt=\"400 Not Found\">"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>Failed to load resource, chabchob server</p>" 
            << "    <!-- </div> -->"
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
        << "\r\n"
        << buffer.c_str();
    }
    return response.str();
}

std::string Response::notImplement()
{
    std::ifstream fi("www/Errors/501.html");
    std::stringstream response;
    response << "HTTP/1.1 501 Not Implemented\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 501 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"
            << "<html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;"
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0; "
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center;" 
            << "            padding: 20px; "
            << "        }"
            << "        .main-content h1 {"
            << "            width: 40%;"
            << "            height: auto;" 
            << "            display: block;" 
            << "            margin: 0 auto;"
            << "            font-size: 40px;"
            << "            color: azure; "
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"main-content\">"
            << "        <h1>501 Not Implemented</h1>"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>transfer encoding is not chanked , chabchob server</p>"
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                << "\r\n"
                << buffer;
    }
    return response.str();
}

std::string Response::httpVersion()
{
    std::ifstream fi("www/Errors/505.html");
    std::stringstream response;
    response << "HTTP/1.1 505 HTTP Version Not Supported\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: chabchoub\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 505 page" << std::endl;
        std::stringstream con ; 

        con << "        <!DOCTYPE html>"
            << "<html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;" 
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0;"
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center;"
            << "            padding: 20px;" 
            << "        }"
            << "        .main-content h1 {"
            << "            width: 40%;"
            << "            height: auto; "
            << "            display: block;" 
            << "            margin: 0 auto;"
            << "            font-size: 40px;"
            << "            color: azure; "
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"main-content\">"
            << "        <h1>505 HTTP Version Not Supported</h1>"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>The server does not support the HTTP protocol version used in the request. Server uses HTTP/1.1</p> "
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                << "\r\n"
                << buffer.c_str();
    }
    return response.str();
}

std::string Response::EntityTooLarge()
{
    std::ifstream fi("www/Errors/413.html");
    std::stringstream response;
    response << "HTTP/1.1 413 Entity too large\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: chabchoub\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in home page" << std::endl;
        std::stringstream con;
        con << "<!DOCTYPE html>"
            << "<html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh; "
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0; "
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center; "
            << "            padding: 20px; "
            << "        }"
            << "        .main-content h1 {"
            << "            width: 40%;"
            << "            height: auto; "
            << "            display: block; "
            << "            margin: 0 auto;"
            << "            font-size: 40px;"
            << "            color: azure; "
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"main-content\">"
            << "        <h1>413 Request Entity too lorg</h1>"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>chabchob server</p>"
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();  
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                << "\r\n"
                << buffer.c_str();
    }
    return response.str();
}

std::string Response::forbidden()
{
    std::ifstream fi("www/Errors/403.html");
    std::stringstream response;
    response << "HTTP/1.1 403 forbidden\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: chabchoub\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 403 page" << std::endl;
        std::stringstream con;
        con << "        <!DOCTYPE html>"
            << "<html>"
            << "<head>"
            << "    <base href=\"/Errors/\" />"
            << "    <style>"
            << "        body {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;"
            << "            background-color: rgb(0, 4, 4);"
            << "            margin: 0;"
            << "        }"
            << "        .main-content {"
            << "            box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "            background-color: hsla(0, 0%, 100%, 0.3);"
            << "            border-radius: 50px;"
            << "            display: block;"
            << "            width: 80%;"
            << "            height: 80%;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            text-align: center;" 
            << "            padding: 20px;" 
            << "        }"
            << "        .main-content img {"
            << "            width: 40%;"
            << "            mix-blend-mode: multiply;"
            << "            height: auto;" 
            << "            display: block;"
            << "            margin: 0 auto;" 
            << "        }"
            << "        .main-content p {"
            << "            color: rgb(255, 0, 0);"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"main-content\">"
            << "        <img src=\"assets/403.gif\" alt=\"403 Forbidden\">"
            << "        <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "        <p>403 Forbidden, chabchob server</p> "
            << "    </div>"
            << "</body>"
            << "</html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                << "\r\n"
                << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                << "\r\n"
                << buffer;
    }
    return response.str();
}

std::string Response::badRequest()
{
    std::ifstream fi("www/Errors/400.html");
    std::stringstream response;
    response << "HTTP/1.1 400 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: "
             << "chabchoub"
             << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in home page" << std::endl;
        std::stringstream con;
        con << "         <!DOCTYPE html>"
            << " <html>"
            << " <head>"
            << "     <style>"
            << "         body {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "             background-color: rgb(0, 4, 4);"
            << "             margin: 0; "
            << "         }"
            << "         .main-content {"
            << "             box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "             background-color: hsla(0, 0%, 100%, 0.3);"
            << "             border-radius: 50px;"
            << "             display: block;"
            << "             width: 80%;"
            << "             height: 80%;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             text-align: center; "
            << "             padding: 20px; "
            << "         }"
            << "         .main-content img {"
            << "             width: 40%;"
            << "             height: auto; "
            << "             display: block; "
            << "             margin: 0 auto;"
            << "         }"
            << "         .main-content p {"
            << "             color: rgb(255, 0, 0);"
            << "         }"
            << "     </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"main-content\">"
            << "         <img src=\"assets/400.svg\" alt=\"400 bad Request\">"
            << "         <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "         <p>400 Invalid URL, chabchob server</p> "
            << "     </div>"
            << " </body>"
            << " </html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                 << "\r\n"
                 << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                 << "\r\n"
                 << buffer.c_str();
    }
    return response.str();
}

std::string Response::longRequest()
{
    std::ifstream fi("www/Errors/414.html");
    std::stringstream response;
    response << "HTTP/1.1 414 Bad Request\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: "
             << "chabchoub"
             << "\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in home page" << std::endl;
        std::stringstream con;

        con << " <!DOCTYPE html>"
            << " <html>"
            << " <head>"
            << "     <style>"
            << "         body {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh; "
            << "             background-color: rgb(0, 4, 4);"
            << "             margin: 0; "
            << "         }"
            << "         .main-content {"
            << "             box-shadow: inset 0px 0px 30px 30px rgb(0, 0, 0);"
            << "             background-color: hsla(0, 0%, 100%, 0.3);"
            << "             border-radius: 50px;"
            << "             display: block;"
            << "             width: 80%;"
            << "             height: 80%;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             text-align: center; "
            << "             padding: 20px; "
            << "         }"
            << "         .main-content h1 {"
            << "             width: 40%;"
            << "             height: auto; "
            << "             display: block; "
            << "             margin: 0 auto;"
            << "             font-size: 40px;"
            << "             color: azure; "
            << "         }"
            << "         .main-content p {"
            << "             color: rgb(255, 0, 0);"
            << "         }"
            << "     </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"main-content\">"
            << "         <h1>414 Request URI, too long</h1>"
            << "         <hr size=\"2\" color=\"white\" width=\"70%\">"
            << "         <p>Invalid URL, chabchob server</p> "
            << "     </div>"
            << " </body>"
            << " </html>";
        response << "Content-Length: " << con.str().size() << "\r\n"
                 << "\r\n"
                 << con.str();
    }
    else
    {
        std::string buffer((std::istreambuf_iterator<char>(fi)), std::istreambuf_iterator<char>());
        response << "Content-Length: " << buffer.size() << "\r\n"
                 << "\r\n"
                 << buffer.c_str();
    }
    return response.str();
}
