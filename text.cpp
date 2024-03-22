

#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 

// #export GATEWAY_INTERFACE="CGI/1.1"                                                                                             
// export REDIRECT_STATUS=200
// export SERVER_PROTOCOL="HTTP/1.1"
// export DOCUMENT_ROOT="/home/rafael/web"
// #export SCRIPT_FILENAME="cgi-bin/test.php"                                                                                      
// export SCRIPT_NAME="cgi-bin/test.php" 

int main()
{
//      = {                             
//    "QUERY_STRING", "CONTET_LENGHT,CONTENT_TYPE"     
//    "REQUEST_METHOD","SCRIPT_FILENAME",
//    "SCRIPT_NAME",  };
    //www/server1/upload/test.cpp
    // getenv("QUERY_STRING")
    std::cout << "---PARET BEFOR FORK------\n";
    // // char *env[]={"PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games",NULL};
    char *args[]={"/usr/local/bin/php-cgi","/www/server1/upload/test.cpp", NULL};
    char *env[7];
    env[0] = "QUERY_STRING = \"IdcService=GET_DOC_PAGE&Action=GetTemplatePage&Page=STD_QUERY_PAGE\"";
    env[1]= "CONTENT_LENGTH = \"1000\"";
    env[2] = "CONTENT_TYPE = \"text/plaingit\"";
    env[3] = "REQUEST_METHOD = \"GET\" ";
    env[4] = "SCRIPT_NAME = \"/www/server1/upload/test.cpp\" ";
    env[5] = "SCRIPT_FILENAME = \"/www/server1/upload/test.cpp\" ";
    int pid = fork();
    if (pid == 0){
        std::cout << "--INSIDE CHILD---\n";
        if (execve(args[0],args,env) < 0){
            perror("execve");
            exit(0);
        }
        // exit(0);
    }
    else if (pid == -1){

        perror("fork :");
        exit(0);
    }

    std::cout << "---PARET AFTER FORK------\n";

}