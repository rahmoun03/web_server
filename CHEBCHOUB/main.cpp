
#include "netplix.hpp"


int main(int ac,char *av[]) 
{

    (void)av;
    (void)ac;

    const char *_null = "NULL";
    if (!av[1])
            netPlix netPlix(_null);
    netPlix netPlix(av[1]);
    return 0;
}
