
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
                    server[serverNum] = conf;
                    std::cout << "---->  : " << server[serverNum].confCherch("server_name") << std::endl;
                     serverNum++;
                    // std::cout << "number of server : " << conf.numOfserver << std::endl;
                    // serverNum = conf.numOfserver;
                }
                fg.close();
                    // exit(0);
            }
            // std::cout << "number of server : " << serverNum << std::endl;
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
                //set fd NON_BLOCKIN WITH FCNTL FUNCTION
                // fcntl(socket_fd,F_SETFL,O_NONBLOCK);
                //NOW CREATE AND INIT EPOLL INSTANCE
                //give fd acce add or del with epoll_ctl function
                // while (1) 
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
                                    event.data.fd = new_socketfd;
                                    event.events = EPOLLIN;
                                    std::cout << GREEN << "Received connection from " << inet_ntoa(clientaddr.sin_addr) << " on fd : "<< new_socketfd << DEF << std::endl;
                                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socketfd, &event);
                        }
                        else if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                            close(fd);
                        }
                        else
                        {
                            std::cout << "---------HERE INSIDE TRY---------\n";
                            try
                            {
                                
                                std::cout << "endOf = " << client[fd].endOf << " ,for fd : "<< fd << std::endl;
                                ssize_t a = -1;
                                if (client[fd].endOf == (size_t)-1)
                                {
                                    char buffer[1024];
                                    if ((a = recv(fd, buffer, 1023, 0)) == -1)
                                    {
                                        std::cout << "------HERE------\n";
                                        std::cerr << "failure in read request !" << std::endl;
                                        exit(1);
                                    }
                                    buffer[a] = '\0';
                                    client[fd].buf.write(buffer, a);
                                    // std::cout << "befor------ \n" << buffer << std::endl;
                                    std::cout <<"read " << a <<" from fd : "<< fd <<std::endl;
                                    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
                                    client[fd].endOf = findEndOfHeaders(buffer, a);
                                    std::cout << "client[fd].endOf now = " << client[fd].endOf << " ,for fd : "<<fd<< std::endl;
                                    
                                }
                                    // exit(0);
                                if((a > -1 && a < 1023) || (client[fd].endOf != (size_t)-1))
                                {
                                    if(!client[fd].toRespons)
                                    {
                                        std:: cout << " <<<<<<<<<<<<<<   Start of Request     >>>>>>>>>>>>>>> " << std::endl;
                                        client[fd].toRespons = true;
                                        std::cout << BLUE << "befor :\n" << client[fd].buf.str()<< DEF << std::endl;
                                        client[fd].req = Request(client[fd].buf, client[fd].endOf);
                                        std::cout << RED << "request :\n"
                                                    << YOLLOW << client[fd].req << DEF << std::endl;
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
                                            epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
                                        }
                                        catch(std::string &content)
                                        {
                                            std::cout<< BLUE<<"respone : \n"<<YOLLOW<< content  << std::endl;
                                            send(fd, content.c_str(), content.size(), 0);
                                        }



                                        std::cout << RED << "Client closed connection" << DEF << std::endl;
                                        if (client[fd].req.connexion == true){
                                            // std::cout << "-------HERE------\n";
                                            close(fd);
                                            client[fd].clear();
                                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
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
                            }
                            // Connection closed or error

                            // close(new_socketfd);
                        }
                    }
                    lop++;
                }
            close(socket_fd[MAX_EVENTS]);
                // close(new_fdsock);
            // exit(0);
        }
};


#endif