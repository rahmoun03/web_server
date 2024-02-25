
#include "../AYOUB/Response.hpp"


const int PORT = 8080;

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t sinSize = sizeof(struct sockaddr_in);

    // Create socket
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cerr << "Error: Failed to create socket\n";
        return 1;
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
    {
        std::cerr << "Error: Failed to set socket options\n";
        return 1;
    }

    // Set address information
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error: Failed to bind socket\n";
        return 1;
    }

    // Listen for connections
    if (listen(serverSocket, 10) == -1)
    {
        std::cerr << "Error: Failed to listen on socket\n";
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(serverSocket, &readSet);
    int count = 1;
    while (true)
    {
        std::cout << "while : " << count << std::endl;
        if (select(FD_SETSIZE, &readSet, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Error: Failed to select\n";
            break;
        }

        for (int fd = 0; fd < FD_SETSIZE; ++fd)
        {
            // std::cout << "fd = " << fd << std::endl;
            if (FD_ISSET(fd, &readSet))
            {
                std::cout << fd << " in the socket list " << std::endl;
                if (fd == serverSocket)
                {
                    // Accept incoming connection
                    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &sinSize)) == -1)
                    {
                        std::cerr << "Error: Failed to accept connection\n";
                    }
                    else
                    {
                        FD_SET(clientSocket, &readSet);
                        fd = clientSocket;
                        std::cout << GREEN << "Received connection from " << inet_ntoa(clientAddr.sin_addr) << " on fd : "<<fd << DEF << std::endl;
                    }
                }

                Request *req = new Request(fd);
                std::cout << BLUE << "request :\n"
                          << YOLLOW << *req << DEF << std::endl;

                // Handle request and send response
                Response *res = new Response(fd, req);

                // Connection closed or error
                close(fd);
                FD_CLR(fd, &readSet);
                delete req;
                delete res;
                std::cout << RED << "Client closed connection" << DEF << std::endl;
            }
        }
        count++;
    }

    // Close server socket
    close(serverSocket);

    return 0;
}