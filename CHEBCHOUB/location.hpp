

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <stdlib.h>
#include "conf.hpp"
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

class location {
    private :
        loca loc;
        std::map<std::string,std::string> loc1;
        std::map<std::string, loca> l;
        // std::string loca;
        std::ifstream& fg;
        std::string name;
    public :
        location(std::ifstream &f,std::string & n) :  fg(f) ,name(n){
        }
        void parsLocation(){
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
        void displayLocation(void){
            std::map<std::string,std::string>::iterator it = loc1.begin();
            int i = 0;
            std::map<std::string,loca> lc;
            for(;it != loc1.end(); it++){
                if (it->first.find("location") != std::string::npos){
                    loc = loca();
                    loc.location = it->second;
                    i++;
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
                l[loc.location] = loc;
            }
        }
        std::map<std::string,loca> getLocation(){
            return l;
        }
};


#endif