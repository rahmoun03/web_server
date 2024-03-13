
#pragma once

#include <fcntl.h>
#include <sys/epoll.h>
#include "conf.hpp"
#include <arpa/inet.h>
#include "../AYOUB/Request.hpp"
#include "../AYOUB/Client.hpp"
#include "../AYOUB/Response.hpp"
#define MAX_EVENTS 1024
// typedef struct  sockaddr_in socketadress;
    // struct sockaddr_in address;

class netPlix : public Conf
{
    private :
        Client client[MAX_EVENTS];
        int socket_fd, new_fdsock; // done
    public :
        netPlix(char *os) : Conf(os){
            // exit(0);
            int opt = 1;
            struct  sockaddr_in socketadress, clientaddr;// done
            socklen_t addrlen = sizeof(socketadress);
            socket_fd = socket(AF_INET,SOCK_STREAM,0);
            setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
            socketadress.sin_family = AF_INET;
            socketadress.sin_port = htons(atoi(this->confCherch("port").c_str()));
            socketadress.sin_addr.s_addr = inet_addr(this->confCherch("host").c_str());
            bind(socket_fd,(struct  sockaddr*)&socketadress,sizeof(socketadress));
            listen(socket_fd,3);
            std::cout<< BLUE << "the server listening on port : "<< DEF << this->confCherch("port") << std::endl;
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
            event.events = EPOLLIN;
            //give fd acce add or del with epoll_ctl function
            if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd,&event) == -1){
                perror("epoll_ctl");
                exit(0);
            }
            for (size_t i = 0; i < MAX_EVENTS; i++)
                client[i].endOf = -1;
            
            int lop = 1;
            std::cout << "---------------------\n";
            while (1)
            {
                std::cout << GREEN << "LOOP = " << lop << DEF <<std::endl;
                //return only sock<<<<<<< HEADet for wich there are events
                int wait_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
                if (wait_fd == -1){
                    perror("epoll_wait");
                    exit(0);
                }
                for (int i = 0; i < wait_fd; i++)
                {
                    int  fd = events[i].data.fd;
                    std::cout << GREEN << "fd = " << fd << DEF << "/"<< wait_fd <<std::endl;
                    if (fd == socket_fd)
                    {
                        //NEW CONNECTION
                            //incoming connection
                        // while (1){
                                int new_socketfd = accept(socket_fd, (struct sockaddr *)&clientaddr, &addrlen);
                                if (new_socketfd == -1){
                                    //check if fd i empty and or fill 
                                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                                    perror("accept");
                                    }
                                    exit(0);
                            
                                }
                                //make the new connection no blocking
                                fcntl(new_socketfd,F_SETFL,O_NONBLOCK);
                                event.events = EPOLLIN;
                                event.data.fd = new_socketfd;
                                std::cout << GREEN << "Received connection from " << inet_ntoa(clientaddr.sin_addr) << " on fd : "<< new_socketfd << DEF << std::endl;
                                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socketfd, &event);
                        // }
                         
                    }
                    // else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)){
                    //     //connection closed;
                    //     std::cout << "-----insidw is--------\n";
                    //     close(fd);
                    // }
                    else
                    {
                        try
                        {
                            
                            std::cout << "endOf = " << client[fd].endOf << " ,for fd : "<< fd << std::endl;
                            ssize_t a = -1;
                            if (client[fd].endOf == (size_t)-1)
                            {
                                char buffer[1024];
                                if ((a = recv(fd, buffer, 1023, 0)) == -1)
                                {
                                    std::cerr << "failure in read request !" << std::endl;
                                    exit(1);
                                }
                                buffer[a] = '\0';
                                client[fd].buf.write(buffer, a);
                                std::cout <<"read " << a <<" from fd : "<< fd <<std::endl;
                                std::cout << "file :\n" << buffer << std::endl;
                                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
                                client[fd].endOf = findEndOfHeaders(buffer, a);
                                if(client[fd].endOf != (size_t)-1)
                                    client[fd].endOf = client[fd].buf.str().size() -  (a - client[fd].endOf);
                                std::cout << "client[fd].endOf now = " << client[fd].endOf << " ,for fd : "<<fd<< std::endl;
                                
                            }
                            if((a > -1 && a < 1023) || (client[fd].endOf != (size_t)-1))
                            {
                                if(!client[fd].toRespons)
                                {
                                    std:: cout << " <<<<<<<<<<<<<<   Start of Request     >>>>>>>>>>>>>>> " << std::endl;
                                    client[fd].toRespons = true;
                                    std::cout << BLUE << "befor :\n" << client[fd].buf.str() << DEF << std::endl;
                                    client[fd].req = Request(client[fd].buf, client[fd].endOf);
                                    client[fd].req.ra += a;
                                    client[fd].req.body_limit = std::atof(this->confCherch("body_size_limit").c_str());
                                    std::cout << RED << "request ::\n"
                                                << YOLLOW << "|"<< client[fd].req<< "||" << DEF << std::endl;
                                    std::cout << "loop : " << lop << std::endl;
                                    // exit(0);
                                    std:: cout << " <<<<<<<<<<<<<<   End of Request     >>>>>>>>>>>>>>> " << std::endl;
                                }
                                // Handle request and send response
                                if(client[fd].toRespons)
                                {
                                    std:: cout << " <<<<<<<<<<<<<<   Start of Response     >>>>>>>>>>>>>>> " << std::endl;
                                    
                                    
                                    
                                    
                                    try
                                    {
                                        client[fd].res.generateResponse(fd, client[fd].req);
                                        if(client[fd].req.get_method() == "GET")
                                            event.events = EPOLLOUT;

                                        if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1)
                                            std::cout << "epoll mod not work" << std::endl;
                                        std::cout << "epoll_ctl : MOD, for fd : " << fd << std::endl;

                                    }
                                    catch(std::string &content)
                                    {
                                        std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
                                        send(fd, content.c_str(), content.size(), MSG_DONTWAIT);
                                        client[fd].req.connexion = true;
                                    }

                                    if(client[fd].req.connexion)
                                    {
                                        std::cout << RED << "Client closed connection => " << DEF <<fd << std::endl;
                                        close(fd);
                                        client[fd].clear();
                                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
                                        event.events = EPOLLIN;
                                    }
                                    std:: cout << " <<<<<<<<<<<<<<   End of Response     >>>>>>>>>>>>>>> " << std::endl;
                                }
                            }
                        }
                        catch(const char *e)
                        {
                            std::cerr << e << '\n';
                        }
                        catch(std::exception &e)
                        {
                            std::cerr << "Error :" << e.what() << '\n';
                            std::cout << RED << "Client closed connection => " << DEF << fd << std::endl;
                            close(fd);
                            client[fd].clear();
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
                            std:: cout << " <<<<<<<<<<<<<<   End of Response     >>>>>>>>>>>>>>> " << std::endl;
                        }
                        // Connection closed or error

                        // close(new_socketfd);
                    }
                }
                lop++;
            }
            close(new_fdsock);
            close(socket_fd);
            // while (1) 
        }
};
