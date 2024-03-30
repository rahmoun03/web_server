/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netplix.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: himejjad <himejjad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:09:09 by himejjad          #+#    #+#             */
/*   Updated: 2024/03/30 06:17:46 by himejjad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef NET_HPP
#define NET_HPP

#include <fcntl.h>
#include <sys/epoll.h>
#include "conf.hpp"
#include <arpa/inet.h>
#include "../AYOUB/Request.hpp"
#include "../AYOUB/Client.hpp"
#include "../AYOUB/Response.hpp"
#define MAX_EVENTS 1024
// #define SOMAXCONN 1024
// typedef struct  sockaddr_in socketadress;
    // struct sockaddr_in address;

class netPlix{
    private :
        // int file;
        Conf conf;
        int clientOut[MAX_EVENTS];
        Client client[MAX_EVENTS];
        int serverNum;
        struct  sockaddr_in socketadress, clientaddr;
        socklen_t addrlen;// done
        struct epoll_event event, events[MAX_EVENTS];
        int socket_fd[MAX_EVENTS];
        int epoll_fd;
        std::vector<int> socket_acc; //, new_fdsock; // done
    
    
    public :
        std::map<int,Conf> server;
        void servClient(int &i, int &fd);
        
        void printServer()
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
                    std::cout << RED << "location : " << BLUE << loc_it->first << std::endl; 
                    std::cout << "  POST          :" << (loc_it->second.post ? "yes" : "no") << std::endl;
                    std::cout << "  GET           :" << (loc_it->second.get ? "yes" : "no") << std::endl;
                    std::cout << "  DELETE        :" << (loc_it->second.delet ? "yes" : "no") << std::endl;
                    std::cout << "  default       :" << loc_it->second.defau << std::endl;
                    std::cout << "  root          :" << loc_it->second.root << std::endl;
                    std::cout << "  upload        :" << loc_it->second.upload << std::endl;
                    std::cout << "  autoindex     :" << (loc_it->second.autoindex ? "yes" : "no") << std::endl;
                    std::cout << "  redirect      :" << loc_it->second.redirect << std::endl;
                    std::cout << "  CGI           :" << (loc_it->second.cgi ? "yes" : "no") << DEF <<std::endl;
                    loc_it++;
                }
            }
        }
        
        netPlix(const char *os)
        {
            int opt = 1;
            int new_socketfd;
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
					// exit(0);
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
            printServer();
            exit(0);
            for (size_t i = 0; i < MAX_EVENTS; i++)
            {
                client[i].endOf = -1;
                clientOut[i] = -1;
            }
            int lop = 1;
            while (1)
            {
                signal(SIGPIPE, SIG_IGN);
                // std::cout << GREEN << "LOOP = " << lop << DEF <<std::endl;

                std::cout << "epoll waiting for events ...\n";
                int wait_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, 5000);
                // std::cout << "epoll : "<< wait_fd << std::endl;
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
                                exit(EXIT_FAILURE);
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
                for (int i = 0; i < wait_fd; i++)
                {
                    int  fd = events[i].data.fd;
                    // std::cout << GREEN << "fd = " << fd << DEF << " / "<< wait_fd <<" ,his event is : "<< (events[i].events == EPOLLIN ? "EPOLLIN" : "EPOLLOUT") <<std::endl;
                    std::vector<int>::iterator it_serv = std::find(socket_acc.begin(),socket_acc.end(),fd);
                    if (it_serv != socket_acc.end())
                    {
                        new_socketfd = accept(fd, (struct sockaddr *)&clientaddr, &addrlen);
                        
                        if (new_socketfd == -1){
                            //check if fd i empty and or fill 
                            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                                    perror("accept");
                            }
                            exit(0);
                        }
                        event.data.fd = new_socketfd;
                        clientOut[new_socketfd] = new_socketfd;
                        event.events = EPOLLIN;
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socketfd, &event);

                        client[new_socketfd].server_index = std::distance(socket_acc.begin(), it_serv);
                        std::string name = server[client[new_socketfd].server_index].confCherch("server_name");
                        std::cout << BLUE << name << GREEN << " Received connection from ==> " << DEF << inet_ntoa(clientaddr.sin_addr) << ", on fd : "<< new_socketfd << DEF << std::endl;
                    }
                    else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                        std::cout << "------{ Error Epoll }-------" << std::endl;
                        std::cout << RED <<"Client disconnected : "<< DEF<< fd<< std::endl;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                            perror("epoll_ctl");
                            exit(EXIT_FAILURE);
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
                                exit(EXIT_FAILURE);
                            }
                            std::cout << GREEN << "reading : "<< bytes_read << " request from : " << DEF << fd <<std::endl;
                        }

                        if (bytes_read == 0) {
                            std::cout << RED << "read 0 "<< DEF << std::endl;
                            std::cout << RED <<"Client disconnected : "<< DEF<< fd<< std::endl;
                            if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                                perror("epoll_ctl");
                                exit(EXIT_FAILURE);
                            }
                            client[fd].clear();
                            close(fd);
                            clientOut[fd] = -1;
                        }
                        else 
                        {
                            if(client[fd].endOf == (size_t)-1 && bytes_read > -1 && events[i].events == EPOLLIN)
                            {
                                buffer[bytes_read] = '\0';
                                std::cout << RAN << "request :\n" << buffer << DEF << std::endl;

                                std::cout << " good()=" << client[fd].buf.good()<< std::endl;
                                std::cout << " eof()=" << client[fd].buf.eof()<< std::endl;
                                std::cout << " fail()=" << client[fd].buf.fail()<< std::endl;
                                std::cout << " bad()=" << client[fd].buf.bad()<< std::endl;
                                
                                /***********************************************************************/

                                client[fd].buf.write(buffer, bytes_read);
                                std::cout << RAN << "buf :\n" << (client[fd].buf.str().empty() ? "empty" : client[fd].buf.str()) << DEF << std::endl;
                                client[fd].endOf = findEndOfHeaders(const_cast<char *>(client[fd].buf.str().c_str()) , (ssize_t)client[fd].buf.str().size());

                                
                                /***********************************************************************/
                                std::cout << "end of file is :"<< (int)client[fd].endOf << std::endl;
                                if(client[fd].endOf != (size_t)-1)
                                {
                                    std::cout << GREEN << "parse the request ... for " << DEF << fd << std::endl;
                                    // client[fd].req = Request(client[fd].buf, client[fd].endOf);
                                    client[fd].req.pars(client[fd].buf, client[fd].endOf);
                            		// std::cout << (client[fd].req.startLineForma ? "yes" : "no") << std::endl;
                                    client[fd].req.ra += (client[fd].buf.str().size() - client[fd].endOf);
                                    client[fd].req.body_limit = std::atof(server[0].confCherch("body_size_limit").c_str());


                                    /***************************/
                                    // std::cout << YOLLOW << "request :\n" << DEF << client[fd].req << std::endl; 
                                    if(client[fd].req.get_method() == "GET")
                                    {
                                        events[i].events = EPOLLOUT;
                                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &events[i]) == -1) {
                                            perror("epoll_ctl");
                                            exit(EXIT_FAILURE);
                                        }
                                        // std::cout << "change event to EPOLLOUT\n";
                                    }
                                    /****************************/
                                    // exit(0);
                                }
                            }
                            // Handle received data
                            // Example: echo back to the client
                            if(client[fd].endOf != (size_t)-1)
                            {
                                try
                                {
                            		std::cout << " correct request "<< std::endl;

                                    client[fd].res.generateResponse(fd, client[fd].req, server[client[fd].server_index], events[i].events);
                                    
                                    
                                    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &events[i]) == -1) {
                                        perror("epoll_ctl");
                                        std::cout << fd << std::endl;
                                        throw (client[fd].res.serverError(server[client[fd].server_index].confCherch("500"), client[fd].req));
                                    }
                                    std::cout << "change event to " << (events[i].events == EPOLLOUT ? "EPOLLOUT" : "EPOLLIN") << std::endl;

                                }
                                catch(std::string &content)
                                {
                                    if(!client[fd].res.firstExcep)
                                    {
                                        std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;

                                        send(fd, content.c_str(), content.size(), 0);
                                        client[fd].req.firstTime = false;
                                        client[fd].res.firstExcep = true;

                                    }
                                    else
                                    {
                                        std::string cont = client[fd].res.getResource(client[fd].res.file, client[fd].req, server[client[fd].server_index]);
                                        std::cout << RAN << cont << DEF << std::endl;
                                        std::cout << YOLLOW << "send response to client " << DEF << std::endl;
                                        send(fd, cont.c_str(), cont.size(), 0);
                                        
                                    }
                                }
                            }
                        }
                        if(client[fd].req.connexion)
                        {
                            std::cout << RED <<"Client disconnected : "<< DEF<< fd << std::endl;
                            if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                                perror("epoll_ctl");
                                exit(EXIT_FAILURE);
                            }
                            client[fd].clear();
                            clientOut[fd] = -1;
                            close(fd);
                        }
                        // servClient(i, fd);
                    }
                }
                lop++;
            }
            close(socket_fd[MAX_EVENTS]);
        }
};


#endif
