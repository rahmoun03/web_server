#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void serveCgi(int &fd){
    // Set up environment variables

    const char* temp_file = "./cgi_output.txt"; // Adjust the path as needed
    FILE* output_file = fopen(temp_file, "w");
    if (!output_file) {
        std::cerr << "Failed to open temporary file for writing." << std::endl;
        return 1;
    }
    setenv("QUERY_STRING", "name=John&age=30", 1); // Example query string
    setenv("REQUEST_METHOD", "GET", 1); // Example request method

    // Execute the CGI script
    pid_t pid = fork();

    if (pid == 0) {
        if (freopen(temp_file, "w", stdout) == NULL) {
            std::cerr << "Failed to freopen stdout." << std::endl;
            return 1;
        }
        // Child process
        const char* script_path = "/usr/bin/php-cgi"; // Path to your CGI script
        // Prepare the argument list
        const char* args[] = {script_path, "index.php", NULL};
        // Prepare the environment variables
        char* const env[] = {NULL}; // You can specify environment variables here if needed
        
        // Execute the CGI script
        if (execve(script_path, (char* const*)args, env) == -1) {
            std::cerr << "Failed to execute CGI script." << std::endl;
            return 1;
        }
    } else if (pid > 0) {
        // Parent process
        // Wait for the child to finish
        waitpid(pid, NULL, 0);
    } else {
        // Fork failed
        std::cerr << "Fork failed." << std::endl;
        return 1;
    }

    return 0;
}
