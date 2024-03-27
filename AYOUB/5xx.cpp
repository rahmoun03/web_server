#include "Response.hpp"

/******************************************************************************/
std::string Response::notImplement(std::string path)
{
    std::ifstream fi(path.c_str());
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
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "     <title>501 Not Implemented</title>"
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
            << "             <h1>501 Not Implemented</h1>"
            << "             <p>The server does not support the functionality required to fulfill the request.</p>"
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

std::string Response::httpVersion(std::string path)
{
    std::ifstream fi(path.c_str());
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
        con << "<!DOCTYPE html>"
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>505 HTTP Version Not Supported</title>"
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
            << "            <h1>505 HTTP Version Not Supported</h1>"
            << "            <p>The server does not support the HTTP protocol version used in the request.</p>"
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

std::string Response::serverError(std::string path)
{
    std::ifstream fi(path.c_str());
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
            << " <html lang=\"en\">"
            << " <head>"
            << "     <meta charset=\"UTF-8\">"
            << "     <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
            << "    <title>500 Internal Server Error</title>"
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
            << "            <h1>500 Internal Server Error</h1>"
            << "            <p>Oops! Something went wrong on the server. We're working to fix it as soon as possible.</p>"
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
