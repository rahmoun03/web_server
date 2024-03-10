

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
// #include "location.hpp"
#include <functional>


class loca{
    public :
        bool get;
        bool post;
        bool delet;
        std::string root;
        std::string location;
        std::string autoindex;
        std::string defau;
        std::string upload;
        std::string redirect;
        // std::string location;
};
class Conf {
    private :
        loca loc;
        std::map<std::string, std::string> map;
        std::string name;
        std::map<std::string, loca>::iterator it;
        std::map<std::string,std::string> loc1;
        std::map<std::string, loca> locat;
        int flag;
    public :
        Conf(){

        }
        Conf(char * os)
        {
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
                    }
                while (getline(fg,name))
                {
                    parsLocation(fg);
                    displayLocation(locat,loc);
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                }
            }
            else
                std::cout << "file not found\n";
        }
        //this for config file data
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
        void parsLocation(std::ifstream & fg){
            if (name.find("location") != std::string::npos)
                {
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    loc1[name.substr(0,name.find("="))] = (name.substr(name.find("=") + 1));
                    if (name.find("[") == std::string::npos)
                    {
                        if (getline(fg,name) && name.find("[") == std::string::npos)
                        {
                            perror("[");
                            exit(0);
                        }
                    }
                    while (getline(fg,name) && name.find("]") == std::string::npos )
                    {
                        loc1[name.substr(0,name.find("="))] = (name.substr(name.find("=") + 1));
                    }
                }
        }
        void displayLocation(std::map<std::string,loca> & tmp,loca &loc){
            std::map<std::string,std::string>::iterator it = loc1.begin();
            for(;it != loc1.end(); it++){
                if (it->first.find("location") != std::string::npos){
                    loc = loca();
                    loc.location = it->second;;
                }
                else
                {
                    if (it->first.find("method") != std::string::npos){
                            loc.get = false;
                            loc.post = false;
                            loc.delet = false;
                            // loca.get = false;
                        if (it->second.find("GET") != std::string::npos)
                                loc.get = true;
                        if (it->second.find("POST") != std::string::npos)
                                loc.post = true;
                        if (it->second.find("DELETE") != std::string::npos)
                                loc.delet = true;
                    }
                    else if (it->first.find("root") != std::string::npos){
                        loc.root = it->second;
                    }
                    else if (it->first.find("autoindex") != std::string::npos){
                        loc.autoindex = it->second;
                    }
                    else if (it->first.find("default") != std::string::npos){
                        loc.defau = it->second;
                    }
                    else if (it->first.find("upload") != std::string::npos){
                        loc.upload = it->second;
                    }
                    else if (it->first.find("redirect") != std::string::npos){
                        loc.redirect = it->second;
                    }
                }
                tmp[loc.location] = loc;
            }
        }
        //this for location data 
        std::map<std::string, loca> getLocal(){
            return locat;
        }
};

#endif