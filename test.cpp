
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string input_string = "           This is a test string with spaces";
    std::stringstream ss(input_string);
    std::string line;

    // Read a line from stringstream with spaces and store it into a string
    std::getline(ss, line);

    std::cout << "Line read from stringstream: " << line << std::endl;

    return 0;
}