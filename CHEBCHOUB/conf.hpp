#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>


class location
{
    private :
        std::map<std::string, std::map<std::string,std::string> > loc;
        //check map for locaton , don't forget
    public :
        location( std::ifstream &fg,std::string name)
        {
            std::cout << "----" << name << std::endl;
            if (name.find("[") == std::string::npos )
            {
                if (getline(fg,name) && name.find("[") == std::string::npos)
                {
                    perror("[");
                    exit(0);
                }
            }
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
            }
            else
                std::cout << "file not found\n";
            
            // close(fg); 
        }

};