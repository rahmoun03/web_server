
#include "WebServer.hpp"



int main(int ac,char *av[]) 
{

    (void)av;
    (void)ac;

    const char *_null = "NULL";
    if (!av[1])
            WebServer WebServer(_null);
    WebServer WebServer(av[1]);
    return 0;
}
