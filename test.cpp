#include <iostream>
#include <dirent.h>

int main() {
    DIR *directory;
    struct dirent *entry;

    // Open the current directory
    if ((directory = opendir(".")) != NULL) {
        // Read directory entries
        while ((entry = readdir(directory)) != NULL) {
            std::cout << entry->d_name  << std::endl;
        }
        closedir(directory);
    } else {
        // Error handling if the directory couldn't be opened
        std::cerr << "Error opening directory" << std::endl;
        return 1;
    }

    return 0;
}