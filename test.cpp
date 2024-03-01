#include <iostream>
#include <sstream>
#include <fstream>

int main(int argc, char const *argv[])
{
    std::ofstream file("txt.txt");
    file << "ayoub\rAY\n";

    // std::cout<< f.str() ;
    return 0;
}
