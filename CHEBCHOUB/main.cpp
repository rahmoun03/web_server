
#include "netplix.hpp"


int main(int ac,char *av[]) 
{

    (void)av;
    if (ac != 2)
        {
            std::cout << "argument less\n";
            exit(0);
        }
    netPlix netPlix(av[1]);
    return 0;
}
