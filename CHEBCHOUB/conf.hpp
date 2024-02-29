// #pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <unistd.h>   //close
#include <netinet/in.h>


class location
{
    private :
        std::map<std::string, std::map<std::string,std::string> > loc;
        std::map<std::string,std::string> loc1;
        int count;
        //check map for locaton , don't forget
    public :
        location( std::ifstream &fg,std::string name)
        {
            (void)fg;
            if (name.find("location") != std::string::npos)
                {
                    count = 0;
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    loc[name.substr(name.find("=") + 1)] = loc1;
                    // loc.erase(name.rfind("["));
                    if (name.find("[") == std::string::npos)
                    {
                        if (getline(fg,name) && name.find("[") == std::string::npos)
                        {
                            perror("[");
                            exit(0);
                        }
                    }
                    while (getline(fg,name) && name.find("]") == std::string::npos &&  name.find("location") == std::string::npos)
                    {
                        // std::cout << name << std::endl;
                        loc1[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                        loc[""] = loc1;

                    }
                    if (name.find("]") == std::string::npos)
                    {
                        perror("error []");
                        exit(0);
                    }
                    // std::map<std::string, std::map<std::string,std::string> >::iterator it = loc.begin();
                    // while (it != loc.end())
                    // {
                    //     std::map<std::string,std::string>::iterator vr = it->second.begin();
                    //     std::cout << "location : " << it->first<< std::endl;
                    //     while (vr != it->second.end())
                    //     {
                    //         std::cout << vr->first << " = " << vr->second << std::endl;
                    //         vr++;
                    //     }
                    //     it++;
                    // }
                    return ;
                }
                // return ;
            // std::cout << "-----------\n";
        }
};
class Conf
{
    private :
        std::map<std::string, std::string> map;
        std::string name;
        int flag;
    public :
        Conf(char * os)
        {
            (void)os;
             std::ifstream fg(os);
             flag  = 0;
            if (fg.is_open())
            {

                getline(fg,name);
                if (name.find("server") == std::string::npos)
                    {
                        perror("server not found");
                        exit(0);
                    }
                if (name.find("{") == std::string::npos )
                    {
                        if (getline(fg,name) && name.find("{") == std::string::npos)
                        {
                            perror("{");
                            exit(0);
                        }
                        flag++;
                    }
                while (getline(fg,name))
                {
                    location loca(fg,name);
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                    // if (name.find("}") != std::string::npos)
                    //     flag++;
                }
                // if (flag != 2){
                //     perror("syntax error");
                //     exit(0);
                // }
            }
            else
                std::cout << "file not found\n";
        }
        std::string confCherch(std::string name)
        {
            std::map<std::string,std::string>::iterator it = map.begin();
            while (it != map.end())
            {
                if (name.compare(it->first) == 0)
                    return it->second;
                it++;
            }
            return "not found";
        }

};
