

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
// #include 
// #include "location.hpp"
#include <functional>
#include "../AYOUB/Response.hpp"
bool directoryExists(std::string path);

class loca{
    public :
        bool get;
        bool post;
        bool delet;
        bool  autoindex;
        std::string root;
        std::string location;
        std::string defau;
        std::string upload;
        std::string redirect;
};

class Conf {
    private :
        // loca loc;
        std::map<std::string, std::string> map;
        std::string name;
        std::map<std::string, loca>::iterator it;
        std::map<std::string,std::string> loc1;
        std::map<std::string,std::string>::iterator ito;
    public :
        std::map<std::string, loca> locat;
        int numOfserver;
        Conf(){
            numOfserver = 0;
        }
        Conf(std::ifstream & fg)
        {
                std::cout << name << std::endl;
                try{
                    getline(fg,name);
                    while (name.empty()){
                        getline(fg,name);
                        if (name.empty()){
                            return ;
                        }
                    }
                    if (name.find("server") != std::string::npos)
                        {
                            numOfserver++;
                            getline(fg,name);
                            if (name.find("{") != std::string::npos)
                                {
                                    while (getline(fg,name)){
                                    if (name.find("=") != std::string::npos){
                                        parsLocation(fg);
                                        if (name.find("]") != std::string::npos){
                                            displayLocation();
                                            continue;
                                        }
                                        name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                                        map[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                                        if (name.find("}") != std::string::npos){
                                            break; 
                                        }
                                        if (name.find("server") != std::string::npos && name.size() == 6){
                                            throw "Ops error config file";
                                        }
                                        }
                                        else if (name.find("}") != std::string::npos){
                                            parsAndCheckServer();
                                            break;
                                        }
                                        else
                                        {
                                            if (!name.empty())
                                                throw "Ops error config file";
                                        }

                                    }
                                }
                                else
                                {
                                    throw "Ops error config file";
                                }
                        }
                        else
                        {
                            throw "Ops error config file";
                        }

                }
                catch (const char * err){
                    std::cout << err << std::endl;
                    exit(0);
                }
                    // displayLocation();

        }
        void    parseAndCheckLocation(){
                std::map<std::string,std::string>::iterator lc;
               try{
                    if (loc1.find("method") == loc1.end()){
                        throw "METHOD NOT FOUND!";
                    }
                    // if ((lc = loc1.find("location")) != loc1.end()){
                    //     if (directoryExists(lc->second) != true){
                    //         throw "LOCATION NOT FOUND!";
                    //     }
                    // }
                    if ((lc = loc1.find("root")) == loc1.end()){
                        throw "ROOT NOT FOUND!";
                    }
                    else{
                            if (directoryExists(lc->second) != true){
                                throw "ROOT NOT FOUND!";
                        }
                    }
                    if ((lc = loc1.find("upload")) != loc1.end()){
                
                            if (directoryExists(lc->second) != true){
                                throw "UPLOAD NOT FOUND!";
                            }
                    }
                lc = loc1.begin();
                for(; lc != loc1.end(); lc++){
                    if (lc->first.compare("cgi") != 0 && lc->first.compare("autoindex") != 0 && lc->first.compare("default") != 0\
                    && lc->first.compare("location") != 0 && lc->first.compare("root") != 0 && lc->first.compare("method") != 0\
                    && lc->first.compare("redirect") != 0 && lc->first.compare("upload") != 0)
                    {

                        throw "SYNTAX ERROR!";
                    }
                }
               }
               catch(const char * err){
                    std::cout << err << std::endl;
                    exit(0);
               }
        }
        void defaultConfic(){
            map["port"] = "8080";
            map["host"] = "127.0.0.1";
            map["server_name"] = "weldjed";
            map["body_size_limit"] = "10000000";
            loc1["location"] = "/";
            loc1["method"] = "GET POST DELETE";
            loc1["root"] = "www/server1/assets";
            this->numOfserver = 1;
        }
        void    parsAndCheckServer(){
            std::map<std::string, std::string>::iterator mp;
            // for(; mp != )
            try{
                if ((mp = map.find("port")) == map.end()){
                        throw "PORT NOT FOUND!";
                }
                else{
                    if (atof(mp->second.c_str()) < 0 | atof(mp->second.c_str()) > 65535)
                        throw "PORT OUT OF RANGE!";
                }
                if (map.find("host") == map.end()){
                        throw "HOST NOT FOUND!";
                }
                if (map.find("body_size_limit") == map.end()){
                        throw "BODY SIZE LIMIT NOT FOUND!";
                }
                if (map.find("root") != map.end()){
                        throw "YOU SHOULD GIVES ROOT IN LOCATION!";
                }

            }
            catch (const char * err){
                std::cout << err << std::endl;
                exit(0);
            }
        }
        std::string confCherch(std::string name)
        {
            if ((ito = map.find(name)) != map.end()){
                return ito->second;
            }
            return "not found";
        }
        void parsLocation(std::ifstream & fg){
            (void)fg;
            int rooty = 0;
            if (name.find("location") != std::string::npos)
                {
                    // displayLocation();
                    loc1.clear();
                    if (name.find("=") != std::string::npos){
                        name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                        loc1[name.substr(0,name.find("="))] = (name.substr(name.find("=") + 1));
                        getline(fg,name);
                        if (name.find("[") != std::string::npos)
                        {
                            while (getline(fg,name)){
                                if (name.find("=") != std::string::npos){
                                    if (name.find("root") != std::string::npos){
                                        rooty++;
                                        if (rooty == 2)
                                            throw "YOU SHOULD ENTER ONE ROOT!";
                                    }
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
                                    parseAndCheckLocation();
                                    // exit(0);
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
        void displayLocation(){
            std::map<std::string,std::string>::iterator it = loc1.begin();
            loca loc = loca();
            // int flag = 0 ;
            for(;it != loc1.end(); it++){
                std::cout << it->first <<" ==== "<< it->second <<std::endl;
                if (it->first.find("location") != std::string::npos){
                    loc.location = it->second;
                    // std::cout << "location name : " << it->second << std::endl;
                    // flag = 1;
                }
                // else
                // {
                else if (it->first.find("method") != std::string::npos){
                        loc.get = false;
                        loc.post = false;
                        loc.delet = false;
                        // loc.autoindex = false;
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
                else if (it->first == "autoindex"){

                        // loc.autoindex  = it->second;
                    if (it->second == "on")
                    {
                        loc.autoindex = true;

                    }
                    if (it->second == "off")
                    {
                        loc.autoindex = false;

                    }
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
                    // flag = 0;
                // }
            }
            locat[loc.location] = loc;
                std::cout << "---------------------------------------\n";
        }
        std::map<std::string, loca> getLocal(){
            return locat;
        }
        ~Conf(){
            // parsAndCheck();
        }
};

#endif
