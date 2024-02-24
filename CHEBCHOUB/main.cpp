#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../AYOUB/Request.hpp"

const int PORT = 8080;

std::string generateResponse() {
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: text/plain\r\n"
             << "Content-Length: 13\r\n"
             << "\r\n"
             << "Hello, World!";
    return response.str();
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t sinSize = sizeof(struct sockaddr_in);

    // Create socket
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error: Failed to create socket\n";
        return 1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
        std::cerr << "Error: Failed to set socket options\n";
        return 1;
    }

    // Set address information
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Failed to bind socket\n";
        return 1;
    }

    // Listen for connections
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error: Failed to listen on socket\n";
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(serverSocket, &readSet);

    while (true) {
        fd_set tmpSet = readSet;
        if (select(FD_SETSIZE, &tmpSet, NULL, NULL, NULL) == -1) {
            std::cerr << "Error: Failed to select\n";
            break;
        }

        for (int fd = 0; fd < FD_SETSIZE; ++fd) {
            if (FD_ISSET(fd, &tmpSet)) {
                if (fd == serverSocket) {
                    // Accept incoming connection
                    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &sinSize)) == -1) {
                        std::cerr << "Error: Failed to accept connection\n";
                    } else {
                        std::cout << "Received connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
                        FD_SET(clientSocket, &readSet);
                    }
                } else {
                    char buffer[1024];
                    int bytesRead = recv(fd, buffer, sizeof(buffer), 0);
                    if (bytesRead <= 0) {
                        // Connection closed or error
                        std::cout << "Client closed connection" << std::endl;
                        close(fd);
                        FD_CLR(fd, &readSet);
                    } else {
                        // Handle request and send response
                        std::string httpResponse = generateResponse();
                        send(fd, httpResponse.c_str(), httpResponse.length(), 0);
                    // }
                }
            }
        }
    }

    // Close server socket
    close(serverSocket);

    return 0;
}