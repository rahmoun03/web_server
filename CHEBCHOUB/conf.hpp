#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>

class Conf
{
    private :
        std::map<std::string, std::string> map;
        std::string name;
        int flag;
    public :
        Conf(char * os)
        {
            // std::cout << os << std::endl;
            (void)os;
             std::ifstream fg(os);
             flag  = 0;
            //  char *sep = " ";

            if (fg.is_open())
            {
                getline(fg,name);
                if (name.find("server") == std::string::npos)
                    {
                        perror("server not found");
                        exit(0);
                    }
                if (name.find("{") == std::string::npos )//|| (getline(fg,name) && name.find("{") == std::string::npos))
                    {
                        if (getline(fg,name) && name.find("{") == std::string::npos)
                        {
                            perror("{");
                            exit(0);
                        }
                    }
                while (getline(fg,name))
                {
                        // std::cout << "-->" << name.substr(0,name.find("="))<< "--" << name.substr(name.find("=") + 1) << std::endl;
                        name.erase(std::remove_if(name.begin(),name.end(),isspace));
                        map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                    if (name.find("location") != std::string::npos)
                    {
                        break;
                    }
                }
                std::map<std::string,std::string>::iterator it = map.begin();
                while (it != map.end())
                {
                    std::cout <<  "|"<< it->first << "|" << "--" << "|" << it->second<< "|" << std::endl;
                    it++;
                }
            }
            else
                std::cout << "file not found\n";
            
            // close(fg); 
        }

};