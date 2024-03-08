

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "conf.hpp"

class location {
    private :
        std::map<std::string,std::string> loc1;
        std::map<std::string, std::map<std::string,std::string> > loc;
        std::ifstream& fg;
        std::string name;
    public :
        location(std::ifstream &f,std::string & n) :  fg(f) ,name(n){
        }
        void parsLocation(){
            if (name.find("location") != std::string::npos)
                {
                    name.erase(std::remove_if(name.begin(),name.end(),isspace),name.end());
                    loc[name.substr(name.find("=") + 1)] = loc1;
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
                        loc1[name.substr(0,name.find("="))] = name.substr(name.find("=") + 1);
                        loc[""] = loc1;
                    }
                }
        }
        void displayLocation(){
            std::cout << "----HERE---------\n";
                std::map<std::string, std::map<std::string,std::string> >::iterator it = loc.begin();
                while (it != loc.end())
                {
                    std::cout << "location : " << it->first<< std::endl;
                    std::map<std::string,std::string>::iterator vr = it->second.begin();
                    while (vr != it->second.end())
                    {
                        std::cout << vr->first << " = " << vr->second << std::endl;
                        vr++;
                    }
                    it++;
                }
        }
};


#endif