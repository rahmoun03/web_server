if (req.firstTime)
    {
        std::cout << "-----POST START HERE && CREATE FILE AND NAME-----\n";
        std::string type = static_cast<std::string>(req.get_header("Content-Type:"));
        type.erase(std::remove_if(type.begin(),type.end(),isspace),type.end());
        type.erase(type.find("/"));
        type.push_back('.');
        std::string path = ("./wwww/upload/"+ type + req.get_header("Content-Type:").substr(req.get_header("Content-Type:").find("/") + 1));
        std::cout << RED << path << DEF << std::endl;
        // std::cout << "-----HERE-----\n";
        out.open(path.c_str());
            std::cout << "----------------is fill---------------------\n";
        if (!req.get_body().empty())
        {
            if (!req.get_header("Transfer-Encoding:").empty()){
                std::cout << "------------INSIDE GETBODY-------------\n";
                chunked = req.get_body();
                // flag = 1;
            }
            else{
                out.write(req.get_body().c_str(),req.get_body().size());
            }
            out.flush();
        }
        req.firstTime = false;
    }
    if (!req.get_header("Transfer-Encoding:").empty()){
            // std::string Data;
            // std::string tmp;
            std::string chunkHeader;
            // static std::string chunked;
            // std::stringstream 
            std::vector<std::string> data;
            static size_t chunkSize;
            // std::string chunked;
            size_t pos;
            // size_t ins;
            if (flag){
                std::cout << "------FRIST TIME HERE ---------\n";
                    //  Data = chunked;
                    if ((pos = chunked.find("\r\n")) != std::string::npos){
                        chunkHeader = chunked.substr(0, pos);
                        chunkSize = hexadecimal(chunkHeader);
                        chunked.erase(0, pos + 2);
                        std::cout << "size of data  ---- \n" << chunked.size() << ": <> : "<< chunkSize << std::endl;
                    }
                    // std::cout << "size of chunked befor \n ====> : " << chunkSize << std::endl;
                    size_t os;
                    if ((os = chunked.find("\r\n")) != std::string::npos){
                        std::cout << chunked.substr(os,os + 2) << std::endl;
                        std::cout << "0-------------0\n";
                    }

                    tmp.append(chunked,chunked.size());
                    // out.write(Data.c_str(),Data.size());
                    // out.flush();

            }
            exit(0);
            ssize_t a = -1;
            char buffer[1024];
            if ((a = recv(fd, buffer, 1023, 0)) == -1)
            {
                std::cerr << "failure in read request !" << std::endl;
                exit(1);
            }
            buffer[a] = '\0';
            tmp.append(buffer,a);
            if (tmp.size() > chunkSize){
                std::cout << "size of tmp && chunked size is \n --- :> " << tmp.size() << "-|-" << chunkSize << std::endl;
                // pos = tmp.find("\r\n");
                if ((pos = tmp.find("\r\n")) != std::string::npos){
                    std::string ss = tmp.substr(pos - 2, pos);
                    // size_t s = ss.find("\r\n");
                    std::cout << "----------TMP BODY IS ----------\n :> " << ss << std::endl;
                //     // if (s != std::string::npos){
                //     // }
                }
                exit(0);
            }
            out.write(tmp.c_str(),tmp.size());
            out.flush();