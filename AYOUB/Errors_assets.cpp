#include <map>
#include <iostream>

std::map<std::string, std::string> ErrorAssets() {
    
    std::map<std::string, std::string> myMap;

    myMap["/Errors/assets/404yellow.svg"] = "./Errors/assets/404yellow.svg";
    myMap["/Errors/assets/404red.svg"] = "./Errors/assets/404red.svg";
    myMap["/Errors/assets/400.svg"] = "./Errors/assets/400.svg";
    
    return myMap;
}
