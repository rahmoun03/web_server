

#ifndef CONF_HPP
#define CONF_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <unistd.h>   //close
#include <netinet/in.h>
#include "location.hpp"
#include <functional>


class Conf {
    private :
        std::map<std::string, std::string> map;
        std::string name;
        // std::vector<location> locat;
        int flag;
    public :
        std::map<std::string, loca> locat;
        std::map<std::string, loca>::iterator it;
        Conf(){

        }
        Conf(char * os)
        {
             std::ifstream fg(os);
             flag  = 0;
            //  int i = 0;
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
                    }
                while (getline(fg,name))
                {
                    location loca(fg,name);
                    loca.parsLocation();
                    loca.displayLocation();
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                    locat = loca.getLocation();
                    // std::cout << "-------------------------------------------\n";
                    // std::cout << locat.begin()->first << " = " << locat.begin()->second.location << std::endl;
                    // i++;
                }
                // setLocal(locat);
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
        // void setLocal(std::vector<location> pop){
        //     locat = pop;
        // }
        // std::vector<location> getLocal(){
        //     return locat;
        // }

};

#endif