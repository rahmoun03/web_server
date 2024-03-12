#include "Response.hpp"

void	Response::GET(int &fd, Request &req)
{
    std::map<std::string, std::string> map = ErrorAssets();
    map_iterator it = map.find(req.get_path());

    if(it != map.end())
        req.get_path() = it->second;
    else
        req.get_path() = (SERVER_ROOT + req.get_path());

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

long long convertHexToDec(std::string hex)
{
    long long decimal;
    std::stringstream ss;

    ss << hex;
    ss >> std::hex >> decimal;
    return (decimal);
}

void	Response::POST(int &fd, Request &req)
{

    (void) fd;
    (void) req;
    if(req.get_header("Transfer-Encoding:").empty())
    {
        if (req.firstTime)
        {
            std::string str = req.get_body();
            out.open("test.mp4", std::ios::binary);
            out.write(str.c_str(), str.size());
            out.flush();
            req.firstTime = false;
        }
        else 
        {
            size_t a;
            char buffer[1024];
            a = recv(fd, buffer, 1023, 0);
            req.ra += a + 1;
            buffer[a] = '\0';
            out.write(buffer, a);
            out.flush();
            std::cout << "read size : " <<req.ra << "\n content length : "<< req.get_header("Content-Length:") <<  std::endl;
            if(req.ra >= (size_t )atoi(req.get_header("Content-Length:").c_str()))
                req.connexion = true;
            // exit(1);
        }
    }
    else if(req.get_header("Transfer-Encoding:") == "chunked")
    {
        // size_t decimal = 0;
        
        std::string line; 
        if (req.firstTime)
        {
            str = req.get_body();
            std::istringstream f(str);
            std::getline(f, line);
            std::cout << line <<std::endl;
            decimal = convertHexToDec(line);
            req.chun++;
            // std::cout << "chunked : " << decimal << std::endl;
            out.open("test.mp4", std::ios::binary);
            str.erase(0,line.size() + 1);
            tmp.append(str.c_str(), str.size());

            // out.write(str.c_str(), str.size());
            // out.flush();
            req.firstTime = false;
        }
        else 
        {
            size_t a;
            char buffer[1024];
            a = recv(fd, buffer, 1023, 0);
            // std::cout <<  "recv read :" <<a << std::endl;
            buffer[a] = '\0';
            tmp.append(buffer, a);
            std::cout << buffer << std::endl;
            if(tmp.size() >= decimal)
            {
                size_t distance = tmp.size() - decimal;
                out.write(tmp.c_str(), decimal);
                out.flush();
                // exit(0);
                // std::cout << "========1========HOW MYCH==========>" << std::endl;
                // exit(1);
                // std::cout << tmp.rfind("\r\n") << " ++ " << decimal + 2 << std::endl;

                tmp = tmp.substr(decimal + 2, distance);
                // std::cout << "|" << tmp << "|"<<  std::endl;
                // exit(1);
                std::istringstream f(tmp);
                std::getline(f, line);
                std::cout << RED<< line << DEF<<std::endl;
                decimal = convertHexToDec(line);
                req.chun++;

                // std::cout << "chunked : " << decimal << std::endl;
                // out.open("test.png", std::ios::binary);
                tmp.erase(0,line.size()+1);
                // std::cout << "tmp size :" << tmp.size() << "\ndecimal : " << decimal <<std::endl;
                // exit(1); 
            }
            if (decimal == 0){
                req.connexion = true;
                std::cout<< "number : " << req.chun << std::endl;
                // out.close();
            }
            // out.write(buffer, a);
        //     if(a < 1023)
        //         out
        //     // exit(1);
        // }
        }
    }
    // std::string str = req.get_body();
    // std::cout << decimal << std::endl;
    // std::cout <<"|" << req.get_body() << "|"  << "lenght: " << str.length() << std::endl;
}

void	Response::DELETE(int &fd, Request &req){
    (void) fd;
    (void) req;
    return;
};