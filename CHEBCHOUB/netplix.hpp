
#pragma once

// #include <iostream>
// #include <string>
// #include <fstream>
// #include <algorithm>
// #include <iostream>
// #include <string>
// #include <fstream>
// #include <algorithm>
// #include <map>
// #include <stdio.h>
// #include <sys/socket.h>
// #include <string.h>   //strlen
// #include <stdlib.h>
// #include <unistd.h>   //close
// #include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "conf.hpp"
#include <arpa/inet.h>
#include "../AYOUB/Request.hpp"
#include "../AYOUB/Response.hpp"
#define MAX_EVENTS 1024
// typedef struct  sockaddr_in socketadress;
    // struct sockaddr_in address;

class netPlix : public Conf
{
    private :
        int socket_fd, new_fdsock;
    public :
        netPlix(char *os) : Conf(os){
            exit(0);
            int opt = 1;
            struct  sockaddr_in socketadress, clientaddr;
            socklen_t addrlen = sizeof(socketadress);
            socket_fd = socket(AF_INET,SOCK_STREAM,0);
            setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
            socketadress.sin_family = AF_INET;
            socketadress.sin_port = htons(atoi(this->confCherch("port").c_str()));
            socketadress.sin_addr.s_addr = inet_addr(this->confCherch("host").c_str());
            bind(socket_fd,(struct  sockaddr*)&socketadress,sizeof(socketadress));
            listen(socket_fd,3);
            //set fd NON_BLOCKIN WITH FCNTL FUNCTION
            fcntl(socket_fd,F_SETFL,O_NONBLOCK);
            if (socket_fd == -1){
                perror("could not create socket");
                exit(0);
            }
            //NOW CREATE AND INIT EPOLL INSTANCE
            struct epoll_event event, events[MAX_EVENTS];
            int epoll_fd = epoll_create1(0);
            if (epoll_fd == -1){
                perror("epoll create");
                exit(0);
            }
            event.data.fd = socket_fd;
            event.events = EPOLLIN | EPOLLET;
            //give fd acce add or del with epoll_ctl function
            if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd,&event) == -1){
                perror("epoll_ctl");
                exit(0);
            }
            while (1)
            {
                //return only socket for wich there are events
                int wait_fd = epoll_wait(epoll_fd,events, MAX_EVENTS,-1);
                if (wait_fd == -1){
                    perror("epoll_wait");
                    exit(0);
                }
                for (int i = 0; i < wait_fd; i++)
                {
                    int  fd = events[i].data.fd;
                    if (fd == socket_fd)
                    {
                        //NEW CONNECTION
                        while (1){
                            //incoming connection
                            int new_socketfd = accept(socket_fd,(struct sockaddr *)&clientaddr,&addrlen);
                            if (new_socketfd == -1){
                                //check if fd i empty and or fill 
                                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                                    break;
                                }
                                else{
                                    perror("accept");
                                    exit(0);
                                }
                            }
                            //make the new connection no blocking
                            fcntl(new_socketfd,F_SETFL,O_NONBLOCK);
                            event.events = EPOLLIN | EPOLLET;
                            event.data.fd = new_socketfd;
                            std::cout << GREEN << "Received connection from " << inet_ntoa(clientaddr.sin_addr) << " on fd : "<< new_socketfd << DEF << std::endl;
                            if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_socketfd,&event) == -1){
                                perror("epoll_ctl : new_socketfd");
                                break;
                            }

                        }
                    }
                    else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))){
                        //connection closed;
                        close(fd);
                    }
                    else
                    {
                        Request *req = new Request(fd);
                        std::cout << BLUE << "request :\n"
                          << YOLLOW << *req << DEF << std::endl;

                        // Handle request and send response
                        Response *res = new Response(fd, req);

                        // Connection closed or error
                        delete req;
                        delete res;
                        std::cout << RED << "Client closed connection" << DEF << std::endl;
                        // close(new_socketfd);
                    }
                }
            }
            close(new_fdsock);
            close(socket_fd);
            // while (1) 
        }
};