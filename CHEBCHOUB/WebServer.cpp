#include "WebServer.hpp"
// #include "../AYOUB/Client.hpp"
// #include "../AYOUB/Client.hpp"
// #include "../AYOUB/Request.hpp"
// #include "../AYOUB/Response.hpp"

void WebServer::printServer()
{
    for (int i = 0; i < serverNum; i++)
    {
        std::map<std::string, std::string>::iterator it = server[i].map.begin();
        std::map<std::string, std::string>::iterator ite = server[i].map.end();
        while (it != ite)
        {
            std::cout << it->first << " = " << it->second << std::endl;
            it++;
        }
        
        std::map<std::string, loca>::iterator loc_it = server[i].locat.begin();
        std::map<std::string, loca>::iterator loc_ite = server[i].locat.end();

        while (loc_it != loc_ite)
        {
            // std::cout << RED << "location : " << BLUE << loc_it->first << std::endl; 
            std::cout << "  POST          :" << (loc_it->second.post ? "yes" : "no") << std::endl;
            std::cout << "  GET           :" << (loc_it->second.get ? "yes" : "no") << std::endl;
            std::cout << "  DELETE        :" << (loc_it->second.delet ? "yes" : "no") << std::endl;
            std::cout << "  default       :" << loc_it->second.defau << std::endl;
            std::cout << "  root          :" << loc_it->second.root << std::endl;
            std::cout << "  upload        :" << loc_it->second.upload << std::endl;
            std::cout << "  autoindex     :" << (loc_it->second.autoindex ? "yes" : "no") << std::endl;
            std::cout << "  redirect      :" << loc_it->second.redirect << std::endl;
            // std::cout << "  CGI           :" << (loc_it->second.cgi ? "yes" : "no") << DEF <<std::endl;
            loc_it++;
        }
    }
}

void WebServer::servClient(int &i, int &fd)
{
    if(client[fd].endOf != (size_t)-1)
    {
        try
        {
            client[fd].res.generateResponse(fd, client[fd].req, server[client[fd].server_index], (events[i].events));                            
                if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &events[i]) == -1) {
                    perror("epoll_ctl");
                    if(client[fd].res.pid != -1)
                    {
                        kill(client[fd].res.pid , SIGTERM);
                        waitpid(client[fd].res.pid, NULL, 0);
                    }
                    client[fd].req.connexion = true;
                    throw (client[fd].res.serverError(server[client[fd].server_index].confCherch("500"), client[fd].req));
                }
                std::cout << "change event to " << (events[i].events == EPOLLOUT ? "EPOLLOUT" : "EPOLLIN") << std::endl;
        }
        catch(bool a)
        {
            if(a)
            {
                std::cout << RED <<"Client disconnected : "<< DEF<< fd << std::endl;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) 
                {
                    
                    perror("epoll_ctl");
                    if(client[fd].res.pid != -1)
                    {
                        kill(client[fd].res.pid, 9);
                        waitpid(client[fd].res.pid, NULL, 0);
                    }
                }
                client[fd].clear();
                clientOut[fd] = -1;
                close(fd);
            }
        }
        catch(std::string &content)
        {
            if(!client[fd].res.firstExcep)
            {
                std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;

                if(send(fd, content.c_str(), content.size(), 0) < 0)
                {
                    perror("send hihih : ");
                    
                    if(client[fd].res.pid != -1)
                    {
                        kill(client[fd].res.pid , SIGTERM);
                        waitpid(client[fd].res.pid, NULL, 0);
                    }
                    client[fd].req.connexion = true;
                }
                client[fd].req.firstTime = false;
                client[fd].res.firstExcep = true;

            }
            else
            {
                std::string cont = client[fd].res.getResource(client[fd].res.file, client[fd].req, server[client[fd].server_index]);
                std::cout << RAN << cont << DEF << std::endl;
                std::cout << YOLLOW << "send response to client " << DEF << std::endl;
                if(send(fd, cont.c_str(), cont.size(), 0) < 0)
                {
                    perror("send : ");
                    
                    if(client[fd].res.pid != -1)
                    {
                        kill(client[fd].res.pid , SIGTERM);
                        waitpid(client[fd].res.pid, NULL, 0);
                    }
                    client[fd].req.connexion = true;
                }
            }
        }
    }
}

void WebServer::createServer(const char * os)
{
        int opt = 1;
        epoll_fd = epoll_create1(0);
        if (epoll_fd == -1){
            perror("epoll create");
            exit(0);
        }
        addrlen = sizeof(socketadress);
        std::ifstream fg(os);
        if (fg.is_open())
        {
            serverNum = 0;
            while (!fg.eof()){
                Conf conf(fg);
                server[serverNum] = conf;
                serverNum = conf.numOfserver;
                std::cout << "num of server os : " << serverNum << std::endl;
            }
            fg.close();
        }
        else{
                conf.defaultConfic();
                conf.displayLocation();
                server[0] = conf;
                serverNum = 1;
        }
        for(int i = 0; i < serverNum; i++)
        {   
            socket_fd[i] = socket(AF_INET,SOCK_STREAM,0);
            socket_acc.push_back(socket_fd[i]);
            setsockopt(socket_fd[i],SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
            socketadress.sin_family = AF_INET;
            socketadress.sin_port = htons(atoi(server[i].confCherch("port").c_str()));
            socketadress.sin_addr.s_addr = inet_addr(server[i].confCherch("host").c_str());
            int bin = bind(socket_fd[i],(struct  sockaddr*)&socketadress,sizeof(socketadress));
            if (bin < 0)
            {
                perror("bind");
                if(serverNum == 1)
                    exit(0);
            }
            listen(socket_fd[i],SOMAXCONN);
            if (socket_fd[i] == -1){
                perror("could not create socket");
                exit(0);
            }
            event.data.fd = socket_fd[i];
            event.events = EPOLLIN ;
            if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd[i],&event) == -1){
                perror("epoll_ctl");
                exit(0);
            }
        }
}


int WebServer::wait_event(){
        std::cout << "epoll waiting for events ...\n";
        int wait_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, 5000);
        if(wait_fd == 0)
        {
            for (int i = 0; i < MAX_EVENTS; i++)
            {
                if(clientOut[i] != -1)
                {
                    client[i].req.firstTime = true;
                    std::string res = client[i].res.timeOut(server[client[i].server_index].confCherch("408"), client[i].req);
                    send(i, res.c_str(), res.size(), 0);
                    std::cout << YOLLOW <<"send response time out ..."<< DEF<< std::endl;
                    std::cout << RED <<"Client disconnected : "<< DEF << clientOut[i] << std::endl;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, i, NULL) == -1) {
                        perror("epoll_ctl");
                    }
                    client[i].clear();
                    close(i);
                    clientOut[i] = -1;
                }
            }
        }
        if (wait_fd == -1){
            perror("epoll_wait");
            exit(0);
        }
    return wait_fd;
}

int WebServer::acccept_newconnection(std::vector<int>::iterator it_serv,int & fd){

    new_socketfd = accept(fd, (struct sockaddr *)&clientaddr, &addrlen);
    
    if (new_socketfd == -1){
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
            perror("accept");
            return 1;
        }
    }
    event.data.fd = new_socketfd;
    clientOut[new_socketfd] = new_socketfd;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socketfd, &event);

    client[new_socketfd].server_index = std::distance(socket_acc.begin(), it_serv);
    std::string name = server[client[new_socketfd].server_index].confCherch("server_name");
    std::cout << BLUE << name << GREEN << " Received connection from ==> " << DEF << inet_ntoa(clientaddr.sin_addr) << ", on fd : "<< new_socketfd << DEF << std::endl;
    return 0;
}

void WebServer::checkRequest(int &fd,ssize_t & bytes_read,int & i,char *buffer)
{
    if(client[fd].endOf == (size_t)-1 && bytes_read > -1 && events[i].events == EPOLLIN)
    {
        buffer[bytes_read] = '\0';
        client[fd].buf.write(buffer, bytes_read);
        std::cout << RAN << "buf :\n" << (client[fd].buf.str().empty() ? "empty" : client[fd].buf.str()) << DEF << std::endl;
        client[fd].endOf = findEndOfHeaders(const_cast<char *>(client[fd].buf.str().c_str()) , (ssize_t)client[fd].buf.str().size());

        
        /***********************************************************************/
        std::cout << "end of file is :"<< (int)client[fd].endOf << std::endl;
        if(client[fd].endOf != (size_t)-1)
        {
            std::cout << GREEN << "parse the request ... for " << DEF << fd << std::endl;
            client[fd].req.pars(client[fd].buf, client[fd].endOf);
            client[fd].req.ra += (client[fd].buf.str().size() - client[fd].endOf);
            client[fd].req.body_limit = std::atof(server[0].confCherch("body_size_limit").c_str());
            std::cout << "----limit is : " << server[0].confCherch("body_size_limit") << std::endl;
            if(client[fd].req.get_method() == "GET")
            {
                events[i].events = EPOLLOUT;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &events[i]) == -1) {
                    
                    perror("epoll_ctl");
                    if(client[fd].res.pid != -1)
                    {
                        kill(client[fd].res.pid , SIGTERM);
                        waitpid(client[fd].res.pid, NULL, 0);
                    }
                    client[fd].req.connexion = true;
                }
            }
        }
    }
}

WebServer::WebServer(const char *os)
{
    createServer(os);
    // printServer();
    for (size_t i = 0; i < MAX_EVENTS; i++)
    {
        client[i].endOf = -1;
        clientOut[i] = -1;
    }
    int lop = 1;
    while (1)
    {
        signal(SIGPIPE, SIG_IGN);
        int wait_fd = wait_event();
        for (int i = 0; i < wait_fd; i++)
        {
            int  fd = events[i].data.fd;
            std::vector<int>::iterator it_serv = std::find(socket_acc.begin(),socket_acc.end(),fd);
            if (it_serv != socket_acc.end())
            {
                if (acccept_newconnection(it_serv,fd) == 1)
                    continue;
            }
            else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                std::cout << RED <<"Client disconnected : "<< DEF<< fd<< std::endl;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) 
                {
                    
                    if(client[fd].res.pid != -1)
                    {
                        kill(client[fd].res.pid, SIGTERM);
                        waitpid(client[fd].res.pid, NULL, 0);
                    }
                    perror("epoll_ctl");
                }
                client[fd].clear();
                close(fd);
                clientOut[fd] = -1;
            }
            else
            {
                std::cout << BLUE << "serv the client : " << DEF << fd << std::endl;
                char buffer[1024];
                ssize_t bytes_read = -1;
                if(events[i].events == EPOLLIN && client[fd].endOf == (size_t)-1)
                {
                    if ((bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0)) == -1) 
                    {
                        
                        perror("recv");
                        if(client[fd].res.pid != -1)
                        {
                            kill(client[fd].res.pid , SIGTERM);
                            waitpid(client[fd].res.pid, NULL, 0);
                        }
                        bytes_read = 0;
                    }
                    std::cout << GREEN << "reading : "<< bytes_read << " request from : " << DEF << fd <<std::endl;
                }

                if (bytes_read == 0) {
                    std::cout << RED << "read 0 "<< DEF << std::endl;
                    std::cout << RED <<"Client disconnected : "<< DEF<< fd<< std::endl;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) 
                    {
                        
                        if(client[fd].res.pid != -1)
                        {
                            kill(client[fd].res.pid, 9);
                            waitpid(client[fd].res.pid, NULL, 0);
                        }
                        perror("epoll_ctl");
                    }
                    client[fd].clear();
                    close(fd);
                    clientOut[fd] = -1;
                }
                else 
                {
                    checkRequest(fd,bytes_read,i,buffer);
                    servClient(i,fd);
                }
                if(client[fd].req.connexion)
                {
                    std::cout << RED <<"Client disconnected : "<< DEF<< fd << std::endl;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) 
                    {
                        
                        perror("epoll_ctl");
                        if(client[fd].res.pid != -1)
                        {
                            kill(client[fd].res.pid, 9);
                            waitpid(client[fd].res.pid, NULL, 0);
                        }
                    }
                    client[fd].clear();
                    clientOut[fd] = -1;
                    close(fd);
                }
            }
        }
        lop++;
    }
    close(socket_fd[MAX_EVENTS]);
}

WebServer::~WebServer(){}

