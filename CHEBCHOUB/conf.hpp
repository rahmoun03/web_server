

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
};
class Conf {
    private :
        loca loc;
        std::map<std::string, std::string> map;
        std::string name;
        std::map<std::string, loca>::iterator it;
        std::map<std::string,std::string> loc1;
        std::map<std::string, loca> locat;
        std::map<std::string,std::string>::iterator ito;
    public :
        int numOfserver;
        Conf(){
            numOfserver = 0;
        }
        Conf(std::ifstream & fg)
        {
                std::cout << name << std::endl;
                try{
                    getline(fg,name);
                    // while (name.empty()){
                    //     getline(fg,name);
                        if (name.empty())
                            return ;
                    // }
                    if (name.find("server") != std::string::npos)
                        {
                            numOfserver++;
                            getline(fg,name);
                            if (name.find("{") != std::string::npos)
                                {
                                    while (getline(fg,name)){
                                    if (name.find("=") != std::string::npos){
                                        parsLocation(fg);
                                        displayLocation(locat,loc);
                                        name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                                        map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                                        if (name.find("}") != std::string::npos)
                                        {
                                            break; 
                                        }
                                        if (name.find("server") != std::string::npos && name.size() == 6){
                                            throw "Ops error config file";
                                        }
                                        }
                                        else if (name.find("}") != std::string::npos){
                                            break;
                                        }
                                        else{
                                            if (!name.empty())
                                                throw "Ops error config file";
                                        }

                                    }
                                }
                                else{
                                    throw "Ops error config file";
                                }
                        }
                        else{
                            throw "Ops error config file";
                        }

                }
                catch (const char * err){
                    std::cout << err << std::endl;
                    exit(0);
                }
        }
        //this for config file data
        std::string confCherch(std::string name)
        {
            if ((ito = map.find(name)) != map.end()){
                return ito->second;
            }
            return "not found";
        }
        void parsLocation(std::ifstream & fg){
            (void)fg;
            if (name.find("location") != std::string::npos)
                {
                    if (name.find("=") != std::string::npos){
                        name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                        loc1[name.substr(0,name.find("="))] = (name.substr(name.find("=") + 1));
                        getline(fg,name);
                        if (name.find("[") != std::string::npos)
                        {
                            while (getline(fg,name)){
                                if (name.find("=") != std::string::npos){
                                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                                    loc1[name.substr(0,name.find("="))] = (name.substr(name.find("=") + 1));
                                    if (name.find("]") != std::string::npos){
                                        break;
                                    }
                                    if (name.find("location") != std::string::npos){
                                            throw "Ops error config file";
                                    }
                                }
                                else if (name.find("]") != std::string::npos){
                                    break;
                                }
                                else{
                                    throw "Ops error config file";
                                }

                            }
                        }
                        else
                            throw "Ops error config file";
                    }
                    else
                        throw "Ops error config file";
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
        std::map<std::string, loca> getLocal(){
        std::map<std::string, loca>::iterator itt = locat.begin();
        for(;itt != locat.end(); itt++){
            std::cout << itt->first << " - - " << itt->second.root << std::endl;
        }
            return locat;
        }
};

#endif
