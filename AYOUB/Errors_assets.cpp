#include <map>
#include <iostream>

std::map<std::string, std::string> ErrorAssets() {
    
    std::map<std::string, std::string> myMap;

    myMap["/Errors/assets/404yellow.svg"] = "./www/Errors/assets/404yellow.svg";
    myMap["/Errors/assets/404red.svg"] = "./www/Errors/assets/404red.svg";
    myMap["/Errors/assets/400.svg"] = "./www/Errors/assets/400.svg";
    myMap["/Errors/assets/403.gif"] = "./www/Errors/assets/403.gif";
    
    return myMap;
}
