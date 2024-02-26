// #include "conf.hpp"
// #include <netinet/in.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/socket.h>
// #include <unistd.h>
#include "netplix.hpp"


int main(int ac,char *av[]) {

    (void)av;
    if (ac != 2)
        {
            std::cout << "argument less\n";
            exit(0);
        }
    // Conf co(av[1]);
    netPlix netPlix(av[1]);
    return 0;
}
