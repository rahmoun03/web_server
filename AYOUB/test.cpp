// //  DIR* dir = opendir(path.c_str());
// //     if (dir != NULL) {
// //         struct dirent* entry;
// //         while ((entry = readdir(dir)) != NULL) {
// //             if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
// //                 std::string filePath = path + "/" + entry->d_name;
// //                 struct stat st;
// //                 if (lstat(filePath.c_str(), &st) == 0) 
// //                 {
// //                     if (S_ISDIR(st.st_mode)) {
// //                         deleteFiles(filePath);
// //                     } else {
// //                         if (unlink(filePath.c_str()) != 0) {
// //                             std::cerr << "Failed to delete file: " << filePath << std::endl;
// //                         }
// //                     }
// //                 }
// //             }
// //         }
// //         closedir(dir);
// //     }

// #include <cstdlib> // For system()
// #include <unistd.h> // For dup2(), close(), fork()
// #include <sys/wait.h> // For waitpid()

// void Response::GET(int &fd, Request &req, Conf &server)
// {
//     (void) server;

//     std::map<std::string, std::string> map = ErrorAssets();
//     map_iterator it = map.find(req.get_path());
//     if(req.firstTime)
//     {
//         std::cout << "old URL : " << req.get_path() << std::endl;
//         if(it != map.end())
//             req.get_path() = it->second;
//         else
//             req.get_path() = (SERVER_ROOT + req.get_path());
//         std::cout << "new URL : " << req.get_path() << std::endl;
//     }
//     if(directoryExists(req.get_path()))
//     {
//         std::cout << "http://{" << req.get_path() << "} \n";
//         std::cout << "the URL is a directory \n";
//         serv_dir(fd, req);
//     }
//     else if (fileExists(req.get_path()))
//     {
//         std::map<std::string , std::string> mime_map = mimeTypes();
//         it = mime_map.find(extension(req.get_path()));
//         if(it != mime_map.end())
//         {
//             if (it->second == "text/html")
//                 executeCGI(fd, req);
//             else
//             {
//                 std::cout << "http://{" << req.get_path() << "} \n";
//                 std::cout << "the URL is a file : " << it->second << std::endl;
//                 serv_file(it, fd, req);
//             }
//         }
//         else
//         {
//             std::cout << "NOT FOUND 404 in MimeTypes"<< std::endl;
//             throw(notFound());
//         }
//     }
//     else
//     {
//         std::cout << "NOT FOUND 404"<< std::endl;
//         throw(notFound());
//     }
// }

// void Response::executeCGI(int &fd, Request &req)
// {
//     // Fork a new process
//     pid_t pid = fork();
//     if (pid == 0) // Child process
//     {
//         // Redirect STDOUT to the client socket
//         dup2(fd, STDOUT_FILENO);
//         close(fd);

//         // Execute the CGI script
//         char* args[] = {NULL}; // You may need to pass arguments to the CGI script
//         char* env[] = {NULL}; // You may need to set environment variables
//         execle(req.get_path().c_str(), req.get_path().c_str(), NULL, env);

//         // If execle fails
//         std::cerr << "Failed to execute CGI script\n";
//         exit(EXIT_FAILURE);
//     }
//     else if (pid > 0) // Parent process
//     {
//         // Wait for the child process to finish
//         int status;
//         waitpid(pid, &status, 0);
//     }
//     else // Fork failed
//     {
//         std::cerr << "Failed to fork process\n";
//         throw internalServerError();
//     }
// }


