#pragma once

#include <iostream>
#include <string>
#include <fstream>
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
                        // map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                        map["ahmed"] = "med";
                        // std::cout << "-->" << name.substr(0,name.find("="))<< "--" << name.substr(name.find("=") + 1) << std::endl;
                    // if (name.find("location") != std::string::npos)
                    // {
                    //     exit(0);

                    // }
                }
                std::map<std::string,std::string>::iterator it;
                while (it != map.end())
                {
                    std::cout << it->first << "--" << it->second << std::endl;
                    std::cout << "-------\n";
                    it++;
                }
            }
            else
                std::cout << "file not found\n";
            
            // close(fg); 
        }

};