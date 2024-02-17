#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../AYOUB/Request.hpp"

#define PORT 8080

int main() {
    int server_fd, new_socket, opt = 1;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024];
    char hello[1024] = {0};
    const char *ok_status = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    // const char *NOT_status = "HTTP/1.1 404 OK\r\nContent-Type: text/html\r\n\r\n";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt,sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 9) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        ssize_t valread = read(new_socket, buffer, sizeof(buffer) - 1);
        if (valread < 0) {
            perror("read error");
            exit(EXIT_FAILURE);
        }
        buffer[valread] = '\0';
        Request *req = new Request(buffer);
        std::cout << "Requist path : " <<  req->get_path() << std::endl;
        // TODO  response
        
        int fd;
        if(req->get_path() == "./")
            fd = open("./test.html", O_RDONLY);
        else
            fd = open(req->get_path().c_str(), O_RDONLY);
        printf("fd = %d\n", fd);
        if(fd < 3)
        {
            perror("open failure ");
            fd = open("./test.html", O_RDONLY);
        }
        ssize_t va = read(fd, hello, 1023);
        printf("Response :\n");
        send(new_socket, ok_status, strlen(ok_status), 0);
        while (va > 0)
        {
            // printf("%s", hello);
            hello[va] = '\0';
            if (send(new_socket, hello, strlen(hello), 0) != (ssize_t)strlen(hello)) {
                perror("send error");
                exit(EXIT_FAILURE);
            }
            va = read(fd, hello, 1023);
        }
        if (va < 0) {
            perror("read error");
            exit(EXIT_FAILURE);
        }
        /**********************************************/
        close(new_socket);
        delete req;
    }
    close(server_fd);
    return 0;
}
