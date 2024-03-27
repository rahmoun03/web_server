#include "Response.hpp"

/******************************************************************************/

std::string Response::lengthRequired(std::string path)
{
    std::ifstream fi(path.c_str());
    std::stringstream response;
    response << "HTTP/1.1 411 Length Required\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 411 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"\
            << " <!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>411 Length Required</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << "<body>"
            << "    <div class=\"container\">"
            << "        <div class=\"error-message\">"
            << "            <h1>411 Length Required</h1>"
            << "            <p>The server refuses to accept the request without a defined Content-Length.</p>"
            << "            <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "        </div>"
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
        // std::cerr << RED << "failure in 408 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"\
            << " <!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "     <title>408 Request Timeout</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"container\">"
            << "         <div class=\"error-message\">"
            << "             <h1>408 Request Timeout</h1>"
            << "             <p>The server timed out waiting for the request.</p>"
            << "             <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "         </div>"
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
                << buffer;
    }
    return response.str();
}


std::string Response::notAllow(std::string method, std::string path)
{
    std::ifstream fi(path.c_str());
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
            << " <!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "     <title>405 Method Not Allowed</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"container\">"
            << "         <div class=\"error-message\">"
            << "             <h1>405 Method Not Allowed</h1>"
            << "             <p>The request method ("<< method <<") is not supported for the requested resource.</p>"
            << "             <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "         </div>"
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
                << buffer;
    }
    return response.str();
}


std::string Response::notFound(std::string path, Request &req)
{
    if(req.firstTime)
    {

        std::stringstream response;
        response << "HTTP/1.1 404 Not Found\r\n"
                << "Content-Type: text/html\r\n"
                << "Connection: close\r\n"
                << "Server: chabchoub\r\n"
                << "Date: " << getCurrentDateTime() << "\r\n";
        file = open(path.c_str(), O_RDONLY);
        if (file == -1)
        {
            std::cerr << RED << "failure in 404 page" << std::endl;
            std::stringstream con ;
            con << "<!DOCTYPE html>"
                << " <html lang=\"en\">"
                << " <head>"
                << "     <meta charset=\"UTF-8\">"
                << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                << "     <title>404 Not Found</title>"
                << "     <style>"
                << "         body {"
                << "             font-family: Arial, sans-serif;"
                << "             margin: 0;"
                << "             padding: 0;"
                << "             background-color: #f5f5f5;"
                << "         }"
                << " "
                << "         .container {"
                << "             display: flex;"
                << "             justify-content: center;"
                << "             align-items: center;"
                << "             height: 100vh;"
                << "         }"
                << " "
                << "         .error-message {"
                << "             text-align: center;"
                << "         }"
                << " "
                << "         h1 {"
                << "             font-size: 48px;"
                << "             color: #333;"
                << "             margin-bottom: 20px;"
                << "         }"
                << " "
                << "         p {"
                << "             font-size: 18px;"
                << "             color: #666;"
                << "             margin-bottom: 20px;"
                << "         }"
                << " "
                << "         .back-link {"
                << "             font-size: 16px;"
                << "             color: #007bff;"
                << "             text-decoration: none;"
                << "         }"
                << "     </style>"
                << " </head>"
                << " <body>"
                << "     <div class=\"container\">"
                << "         <div class=\"error-message\">"
                << "             <h1>404 Not Found</h1>"
                << "             <p>The requested URL was not found on this server. Please check the URL or go back to the homepage.</p>"
                << "             <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
                << "         </div>"
                << "     </div>"
                << " </body>"
                << " </html>";

            response << "Content-Length: " << con.str().size() << "\r\n"
                    << "\r\n"
                    << con.str();
            req.connexion = true;
        }
        else
        {
            std::ifstream ff(path.c_str(), std::ios::binary);

            ff.seekg(0, std::ios::end);
            std::streampos size = ff.tellg();
            ff.seekg(0, std::ios::beg);
            ff.close();

            response << "Content-Length: " << size << "\r\n"
            << "\r\n";
        }
        return response.str();
    }
    return "";
}

std::string Response::EntityTooLarge(std::string path)
{
    std::ifstream fi(path.c_str());
    std::stringstream response;
    response << "HTTP/1.1 413 Entity too large\r\n"
             << "Content-Type: text/html\r\n"
             << "Connection: close\r\n"
             << "Server: chabchoub\r\n"
             << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in home page" << std::endl;
        std::stringstream con ;
        con << "<!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>413 Payload Too Large</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"container\">"
            << "         <div class=\"error-message\">"
            << "             <h1>413 Payload Too Large</h1>"
            << "             <p>The request is larger than the server is willing or able to process.</p>"
            << "             <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "         </div>"
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

std::string Response::forbidden(std::string path)
{
    std::ifstream fi(path.c_str());
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
        con << "<!DOCTYPE html>"
            << "<html lang=\"en\">"
            << "<head>"
            << "    <meta charset=\"UTF-8\">"
            << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>403 Forbidden</title>"
            << "    <style>"
            << "        body {"
            << "            font-family: Arial, sans-serif;"
            << "            margin: 0;"
            << "            padding: 0;"
            << "            background-color: #f5f5f5;"
            << "        }"
            << ""
            << "        .container {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;"
            << "        }"
            << ""
            << "        .error-message {"
            << "            text-align: center;"
            << "        }"
            << ""
            << "        h1 {"
            << "            font-size: 48px;"
            << "            color: #333;"
            << "            margin-bottom: 20px;"
            << "        }"
            << ""
            << "        p {"
            << "            font-size: 18px;"
            << "            color: #666;"
            << "            margin-bottom: 20px;"
            << "        }"
            << ""
            << "        .back-link {"
            << "            font-size: 16px;"
            << "            color: #007bff;"
            << "            text-decoration: none;"
            << "        }"
            << "    </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"container\">"
            << "         <div class=\"error-message\">"
            << "             <h1>403 Forbidden</h1>"
            << "             <p>You don't have permission to access the requested resource on this server.</p>"
            << "             <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "         </div>"
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
                << buffer;
    }
    return response.str();
}

std::string Response::badRequest(std::string path)
{
    std::ifstream fi(path.c_str());
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
        con << " <!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "     <title>400 Bad Request</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << " <body>"
            << "     <div class=\"container\">"
            << "         <div class=\"error-message\">"
            << "             <h1>400 Bad Request</h1>"
            << "             <p>The server cannot process the request due to a client error. Please check the URL or request format.</p>"
            << "             <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "         </div>"
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

std::string Response::longRequest(std::string path)
{
    std::ifstream fi(path.c_str());
    std::stringstream response;
    response << "HTTP/1.1 414 URI Too Long\r\n"
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
            << "<!DOCTYPE html>"
            << "<html lang=\"en\">"
            << "<head>"
            << "    <meta charset=\"UTF-8\">"
            << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>414 URI Too Long</title>"
            << "    <style>"
            << "        body {"
            << "            font-family: Arial, sans-serif;"
            << "            margin: 0;"
            << "            padding: 0;"
            << "            background-color: #f5f5f5;"
            << "        }"
            << ""
            << "        .container {"
            << "            display: flex;"
            << "            justify-content: center;"
            << "            align-items: center;"
            << "            height: 100vh;"
            << "        }"
            << ""
            << "        .error-message {"
            << "            text-align: center;"
            << "        }"
            << ""
            << "        h1 {"
            << "            font-size: 48px;"
            << "            color: #333;"
            << "            margin-bottom: 20px;"
            << "        }"
            << ""
            << "        p {"
            << "            font-size: 18px;"
            << "            color: #666;"
            << "            margin-bottom: 20px;"
            << "        }"
            << ""
            << "        .back-link {"
            << "            font-size: 16px;"
            << "            color: #007bff;"
            << "            text-decoration: none;"
            << "        }"
            << "    </style>"
            << "</head>"
            << "<body>"
            << "    <div class=\"container\">"
            << "        <div class=\"error-message\">"
            << "            <h1>414 URI Too Long</h1>"
            << "            <p>The URI provided was too long for the server to process.</p>"
            << "            <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "        </div>"
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

std::string Response::mediaType(std::string path)
{
    std::ifstream fi(path.c_str());
    std::stringstream response;
    response << "HTTP/1.1 415 Unsupported Media Type\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 415 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"\
            << " <!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>415 Unsupported Media Type</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << "<body>"
            << "    <div class=\"container\">"
            << "        <div class=\"error-message\">"
            << "            <h1>415 Unsupported Media Type</h1>"
            << "            <p>The server does not support the media type provided in the request.</p>"
            << "            <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "        </div>"
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

std::string Response::headerTooLarge(std::string path)
{
    std::ifstream fi(path.c_str());
    std::stringstream response;
    response << "HTTP/1.1 431 Request Header Fields Too Large\r\n"
            << "Content-Type: text/html\r\n"
            << "Connection: close\r\n"
            << "Server: chabchoub\r\n"
            << "Date: " << getCurrentDateTime() << "\r\n";
    if (!fi.is_open())
    {
        std::cerr << RED << "failure in 431 page" << std::endl;
        std::stringstream con ; 
        con << "<!DOCTYPE html>"\
            << " <!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>431 Request Header Fields Too Large</title>"
            << "     <style>"
            << "         body {"
            << "             font-family: Arial, sans-serif;"
            << "             margin: 0;"
            << "             padding: 0;"
            << "             background-color: #f5f5f5;"
            << "         }"
            << " "
            << "         .container {"
            << "             display: flex;"
            << "             justify-content: center;"
            << "             align-items: center;"
            << "             height: 100vh;"
            << "         }"
            << " "
            << "         .error-message {"
            << "             text-align: center;"
            << "         }"
            << " "
            << "         h1 {"
            << "             font-size: 48px;"
            << "             color: #333;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         p {"
            << "             font-size: 18px;"
            << "             color: #666;"
            << "             margin-bottom: 20px;"
            << "         }"
            << " "
            << "         .back-link {"
            << "             font-size: 16px;"
            << "             color: #007bff;"
            << "             text-decoration: none;"
            << "         }"
            << "     </style>"
            << " </head>"
            << "<body>"
            << "    <div class=\"container\">"
            << "        <div class=\"error-message\">"
            << "            <h1>431 Request Header Fields Too Large</h1>"
            << "            <p>The server is refusing to process a request because the request header fields are too large.</p>"
            << "            <a href=\"/\" class=\"back-link\">Go back to the homepage</a>"
            << "        </div>"
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
