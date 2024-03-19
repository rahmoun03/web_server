
#include "netplix.hpp"


int main(int ac,char *av[]) 
{

    (void)av;
    (void)ac;
    // if (ac != 2)
    //     {
    //         std::cout << "argument less\n";
    //         exit(0);
    //     }
    const char *_null = "NULL";
    if (!av[1])
            netPlix netPlix(_null);
    netPlix netPlix(av[1]);
    return 0;
}
