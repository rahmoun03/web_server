

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

class Conf {
    private :
        std::map<std::string, std::string> map;
        std::string name;
        std::vector<location> locat;
        int flag;
    public :
        Conf(){

        }
        Conf(char * os)
        {
             std::ifstream fg(os);
             flag  = 0;
             int i = 0;
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
                    // exit(0);
                    if (name.find("location") != std::string::npos){
                        std::cout << "position is --> " << name << std::endl;
                        location loca(fg,name);
                        locat.push_back(loca);
                        locat[i].parsLocation();
                        locat[i].displayLocation();
                        i++;
                    }
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                }
                // setMap(map);    
            }
            else
                std::cout << "file not found\n";
            
            // close(fg); 
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

#endif