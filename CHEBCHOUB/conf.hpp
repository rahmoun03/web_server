<<<<<<< HEAD
// #pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
=======
#pragma once

>>>>>>> origin/chebchoub
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
<<<<<<< HEAD
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <unistd.h>   //close
#include <netinet/in.h>
=======
>>>>>>> origin/chebchoub


class location
{
    private :
        std::map<std::string, std::map<std::string,std::string> > loc;
<<<<<<< HEAD
        std::map<std::string,std::string> loc1;
        int count;
=======
>>>>>>> origin/chebchoub
        //check map for locaton , don't forget
    public :
        location( std::ifstream &fg,std::string name)
        {
<<<<<<< HEAD
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
<<<<<<< HEAD
                // std::map<std::string, std::map<std::string,std::string> >::iterator it = loc.begin();
                // while (it != loc.end())
                // {
                //     std::cout << "location : " << it->first<< std::endl;
                //     std::map<std::string,std::string>::iterator vr = it->second.begin();
                //     while (vr != it->second.end())
                //     {
                //         std::cout << vr->first << " = " << vr->second << std::endl;
                //         vr++;
                //     }
                //     it++;
                // }
=======
            std::cout << "----" << name << std::endl;
            if (name.find("[") == std::string::npos )
            {
                if (getline(fg,name) && name.find("[") == std::string::npos)
                {
                    perror("[");
                    exit(0);
                }
            }
>>>>>>> origin/chebchoub
=======
                // return ;
            // std::cout << "-----------\n";
>>>>>>> origin/chebchoub
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
<<<<<<< HEAD
            (void)os;
             std::ifstream fg(os);
             flag  = 0;
            if (fg.is_open())
            {

=======
            // std::cout << os << std::endl;
            (void)os;
             std::ifstream fg(os);
             flag  = 0;
            //  char *sep = " ";

            if (fg.is_open())
            {
>>>>>>> origin/chebchoub
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
<<<<<<< HEAD
                    location loca(fg,name);
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                    // if (name.find("}") != std::string::npos)
                    //     flag++;
                }
<<<<<<< HEAD
                // setMap(map);    

=======
                        name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                        map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                    if (name.find("location") != std::string::npos)
                    {
                        location loca(fg,name);
                        break;
                    }
                }
                std::map<std::string,std::string>::iterator it = map.begin();
                while (it != map.end())
                {
                    std::cout <<  "|"<< it->first << "|" << "--" << "|" << it->second<< "|" << std::endl;
                    it++;
                }
>>>>>>> origin/chebchoub
=======
                // if (flag != 2){
                //     perror("syntax error");
                //     exit(0);
                // }
>>>>>>> origin/chebchoub
            }
            else
                std::cout << "file not found\n";
        }
<<<<<<< HEAD
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
=======

};
>>>>>>> origin/chebchoub
