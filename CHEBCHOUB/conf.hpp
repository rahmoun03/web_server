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
    public :
        Conf(char * os)
        {
            std::cout << os << std::endl;
            (void)os;
             std::ifstream fg(os);
            if (fg.is_open())
            {
                while (getline(fg,name))
                {
                    if (name.find("server") != std::string::npos)
                    {
                        std::cout << name << std::endl;
                        exit(0);
                    }
                    std::cout << name << std::endl;
                }
            }
            else
                std::cout << "file not found\n";
            
            // close(fg); 
        }

};