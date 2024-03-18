
#ifndef NET_HPP
#define NET_HPP

#include <fcntl.h>
#include <sys/epoll.h>
#include "conf.hpp"
// #include "location.hpp"
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
        Conf conf;
        Client client[MAX_EVENTS];
        std::map<int,Conf> server;
        int serverNum;
        int socket_fd[MAX_EVENTS];
        std::vector<int> socket_acc; //, new_fdsock; // done
            struct  sockaddr_in socketadress, clientaddr;
            socklen_t addrlen;// done
        // std::vector<location> loca;
    public :
        netPlix(char *os)
        {
            struct epoll_event event, events[MAX_EVENTS];
            int opt = 1;
            int new_socketfd;
            int epoll_fd = epoll_create1(0);
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
                    conf.getLocal();
                    // conf.parsAndCheckServer();
                    server[serverNum] = conf;
                    serverNum = conf.numOfserver;
                }
                fg.close();
            }
            // server[0].parsAndCheck();
            // for (int p = 0; p < serverNum; ++p){
            //     std::cout << "-> : " << server[p].confCherch("port") << std::endl;
            // }
            // std::cout << "number of server : " << serverNum << std::endl;
            // exit(0);
            for(int i = 0; i < serverNum; i++){
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
                event.events = EPOLLIN | EPOLLET;
                std::cout << server[i].confCherch("port") << std::endl;
                if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket_fd[i],&event) == -1){
                    perror("epoll_ctl");
                    exit(0);
                }
                std::cout<< BLUE << "the server listening on port : "<< DEF << server[i].confCherch("port") << std::endl;
            }
            
            for (size_t i = 0; i < MAX_EVENTS; i++)
                client[i].endOf = -1;
                                int lop = 1;
                std::cout << "---------------------\n";
                while (1)
                {
                    std::cout << GREEN << "LOOP = " << lop << DEF <<std::endl;
                    std::cout << "------WAIT NEW CONNECTION------\n";
                    //return only sock<<<<<<< HEADet for wich there are events
                    int wait_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
                    if (wait_fd == -1){
                        perror("epoll_wait");
                        exit(0);
                    }
                    std::cout << "wait fd is --- : " << wait_fd << std::endl;
                    for (int i = 0; i < wait_fd; i++)
                    {
                        int  fd = events[i].data.fd;
                        std::cout << GREEN << "fd = " << fd << DEF << "/"<< wait_fd <<std::endl;
                        if (std::find(socket_acc.begin(),socket_acc.end(),fd) != socket_acc.end())
                        {
                                    new_socketfd = accept(fd, (struct sockaddr *)&clientaddr, &addrlen);
                                    if (new_socketfd == -1){
                                        //check if fd i empty and or fill 
                                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                                                perror("accept");
                                        }
                                        exit(0);
                                
                                    }
                                    std::cout << "-----ACC NEW CONNEXOIN-----\n";
                                    std::cout << "SERVE NAME IS : " << server[std::distance(socket_acc.begin(),std::find(socket_acc.begin(),socket_acc.end(),fd))].confCherch("server_name");
                                    event.data.fd = new_socketfd;
                                    event.events = EPOLLIN;
                                    std::cout << GREEN << "Received connection from " << inet_ntoa(clientaddr.sin_addr) << " on fd : "<< new_socketfd << DEF << std::endl;
                                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socketfd, &event);
                        }
                        else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
                            close(fd);
                        }
                        else
                        {
                            std::cout << GREEN << "--------------START REQ---------------\n" << DEF;
                            ssize_t a = -1;
                            char buffer[1024];
                            if ((a = recv(fd, buffer, 1023, 0)) == -1)
                            {
                                perror("read : ");
                                std::cerr << "failure in read request for : "<< fd << std::endl;
                                exit(1);
                            }
                            buffer[a] = '\0';
                            client[fd].buf.write(buffer, a);
                            std::cout << "------BODY IS \n " << buffer << std::endl;
                            
                            std::ifstream file("www/server1/index.html");
                            std::stringstream response;
                            response << "HTTP/1.1 200 OK\r\n"
                            << "Content-Type: text/html\r\n"
                            << "Connection: close\r\n"
                            << "Server: " << "chabchoub" << "\r\n"
                            << "Date: " << getCurrentDateTime() << "\r\n"
                            // << "Content-Length: "<< "21" <<"\r\n"
                            << "\r\n"
                            << file.rdbuf();
                            send(fd,response.str().c_str(),response.str().size(), 0);
                            std::cout << "response :\n" << response.str();
                            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
                            close(fd);
                            file.close();

                    }
                }
                    lop++;
                }
            close(socket_fd[MAX_EVENTS]);
        }
};


#endif