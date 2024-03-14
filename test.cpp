#include <iostream>
#include <dirent.h>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file("CHEBCHOUB/egg.png", std::ios::binary);
    

    if(!file.is_open())
    {
        std::cerr << "faile to open the file" << std::endl;
        // exit(1);
    }
    // file.seekg(0, std::ios::end);
    // std::cout << "size of file is : " <<  << std::endl;
    // std::cout << "\n" << file.rdbuf() << std::endl;

    // while (1)
    // {
    //     char buffer[10];
    //     file.read(buffer, 9);

    //     std::stringstream content;
    //     content.write(buffer, 9);
    // }
    




    // DIR *directory;
    // struct dirent *entry;

    // // Open the current directory
    // if ((directory = opendir(".")) != NULL) {
    //     // Read directory entries
    //     while ((entry = readdir(directory)) != NULL) {
    //         std::cout << entry->d_name  << std::endl;
    //     }
    //     closedir(directory);
    // } else {
    //     // Error handling if the directory couldn't be opened
    //     std::cerr << "Error opening directory" << std::endl;
    //     return 1;
    // }

    return 0;
}