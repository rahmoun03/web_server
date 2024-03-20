/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netplix.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbajaou <ahbajaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:09:09 by himejjad          #+#    #+#             */
/*   Updated: 2024/03/19 03:21:06 by ahbajaou         ###   ########.fr       */
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
                    // conf.getLocal();
                    // conf.parsAndCheckServer();
                    server[serverNum] = conf;
                    // std::cout << "---->  : " << server[serverNum].confCherch("server_name") << std::endl;
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
            // std::cout << server[0].locat.find("/assets/")->second.get << std::endl;
            // std::cout << "autoindex : " << server[0].locat.find("/assets/")->second.autoindex << std::endl;
            // std::cout << "autoindex : " << server[0].locat.find("/assets/")->second.defau << std::endl;
            // std::cout << "autoindex : " << server[0].locat.find("/assets/")->second.root << std::endl;
            // std::cout << server[0].locat.find("/assets/")->second.post << std::endl;          

            std::cout << "number of server : " << serverNum << std::endl;
            for(int i = 0; i < serverNum; i++)
            {
                std::cout << "---INSIDE LOOP CREATE SOCKET---\n";
                socket_fd[i] = socket(AF_INET,SOCK_STREAM,0);
                socket_acc.push_back(socket_fd[i]);
                setsockopt(socket_fd[i],SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
                socketadress.sin_family = AF_INET;
                socketadress.sin_port = htons(atoi(server[i].confCherch("port").c_str()));
                socketadress.sin_addr.s_addr = inet_addr(server[i].confCherch("host").c_str());
                bind(socket_fd[i],(struct  sockaddr*)&socketadress,sizeof(socketadress));
                listen(socket_fd[i],SOMAXCONN);
                if (socket_fd[i] == -1){
                    perror("could not create socket");
                    exit(0);
                }
                event.data.fd = socket_fd[i];
                event.events = EPOLLIN ;
                std::cout << server[i].confCherch("port") << std::endl;
                if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd[i],&event) == -1){
                    perror("epoll_ctl");
                    exit(0);
                }
                std::cout << "num of servre is " << i << std::endl;
                std::cout<< BLUE << "the server listening on port : "<< DEF << server[i].confCherch("port") << std::endl;
            }
            
            for (size_t i = 0; i < MAX_EVENTS; i++)
                client[i].endOf = -1;
            int lop = 1;
            std::cout << "---------------------\n";
            while (1)
            {
                std::cout << GREEN << "LOOP = " << lop << DEF <<std::endl;
                std::cout << "epoll waiting for events ...\n";
                //return only socket for wich there are events
                int wait_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
                if (wait_fd == -1){
                    perror("epoll_wait");
                    exit(0);
                }
                for (int i = 0; i < wait_fd; i++)
                {
                    int  fd = events[i].data.fd;
                    std::cout << GREEN << "fd = " << fd << DEF << " / "<< wait_fd <<" ,his event is : "<< (events[i].events == EPOLLIN ? "EPOLLIN" : "EPOLLOUT") <<std::endl;
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
                        event.events = EPOLLIN;
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socketfd, &event);

                        client[new_socketfd].server_index = std::distance(socket_acc.begin(), it_serv);
                        std::string name = server[client[new_socketfd].server_index].confCherch("server_name");
                        std::cout << GREEN << name << " Received connection from " << inet_ntoa(clientaddr.sin_addr) << " on fd : "<< new_socketfd << DEF << std::endl;
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
                    }
                    else
                    {
                        std::cout << BLUE << "serv the client : " << DEF << fd << std::endl;
                        char buffer[1024];
                        ssize_t bytes_read = -1;
                        if(events[i].events == EPOLLIN && client[fd].endOf == (size_t)-1)
                        {
                            std::cout << GREEN << "reading request from : " << DEF << fd <<std::endl;
                            if ((bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0)) == -1) 
                            {
                                perror("recv");
                                exit(EXIT_FAILURE);
                            }
                        }


                        if (bytes_read == 0) {
                            // Client disconnected
                            std::cout << RED <<"Client disconnected : "<< DEF<< fd<< std::endl;
                            if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                                perror("epoll_ctl");
                                exit(EXIT_FAILURE);
                            }
                            client[fd].clear();
                            close(fd);
                        }
                        else 
                        {
                            if(client[fd].endOf == (size_t)-1 && bytes_read > -1 && events[i].events == EPOLLIN)
                            {
                                buffer[bytes_read] = '\0';
                                std::cout << RAN << "request :\n" << buffer << DEF << std::endl;
                                
                                /***********************************************************************/

                                client[fd].buf.write(buffer, bytes_read);
                                client[fd].endOf = findEndOfHeaders(buffer, bytes_read);
                                if(client[fd].endOf != (size_t)-1)
                                    client[fd].endOf = client[fd].buf.str().size() -  (bytes_read - client[fd].endOf);
                                
                                /***********************************************************************/
                                if(client[fd].endOf != (size_t)-1)
                                {
                                    std::cout << GREEN << "parse the request ... for " << DEF << fd << std::endl;
                                    client[fd].req = Request(client[fd].buf, client[fd].endOf);
                                    client[fd].req.ra += (client[fd].buf.str().size() - client[fd].endOf);
                                    client[fd].req.body_limit = std::atof(server[0].confCherch("body_size_limit").c_str());

                                    /***************************/
                                    // std::cout << YOLLOW << "request :\n" << DEF << client[fd].req << std::endl; 
                                    if((client[fd].req.get_method() == "GET")
                                        || (client[fd].req.get_method() == "POST"
                                            && server[client[fd].server_index].locat.find(client[fd].req.get_path())->second.upload.empty()))
                                    {
                                        events[i].events = EPOLLOUT;
                                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &events[i]) == -1) {
                                            perror("epoll_ctl");
                                            exit(EXIT_FAILURE);
                                        }
                                        std::cout << "change event to EPOLLOUT\n";
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
                                    client[fd].res.generateResponse(fd, client[fd].req, events[i].events, server[client[fd].server_index]);
                                }
                                catch(std::string &content)
                                {
                                    std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
                                    send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
                                    client[fd].req.connexion = true;
                                }
                            }
                        }
                        if(client[fd].req.connexion)
                        {
                            std::cout << RED <<"Client disconnected : "<< DEF<< fd<< std::endl;
                            if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                                perror("epoll_ctl");
                                exit(EXIT_FAILURE);
                            }
                            client[fd].clear();
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
