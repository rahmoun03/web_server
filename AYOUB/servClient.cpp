#include "../CHEBCHOUB/netplix.hpp"

void netPlix::servClient(int &i, int &fd)
{
    std::cout << RED << "serv the client " << DEF << std::endl;
    char buffer[1024];
    ssize_t bytes_read = -1;
    if(events[i].events == EPOLLIN)
    {
        if ((bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0)) == -1) 
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }
    }


    if (bytes_read == 0) {
        // Client disconnected
        printf("Client disconnected\n");
        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
        }
        close(fd);
    }
    else 
    {
        if(client[fd].endOf == (size_t)-1 && bytes_read < 1023 && bytes_read > -1 && events[i].events == EPOLLIN)
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
                client[fd].req = Request(client[fd].buf, client[fd].endOf);
                client[fd].req.ra += client[fd].buf.str().size();
                client[fd].req.body_limit = std::atof(server[0].confCherch("body_size_limit").c_str());

                /***************************/
                if(client[fd].req.get_method() == "GET")
                {
                    events[i].events = EPOLLOUT;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &events[i]) == -1) {
                        perror("epoll_ctl");
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "change event to EPOLLOUT\n";
                }
                /****************************/

                exit(0);
            }
        }
        // Handle received data
        // Example: echo back to the client
        else
        {
            client[fd].res.generateResponse(fd, client[fd].req, events[i].events);
        }
    }
}