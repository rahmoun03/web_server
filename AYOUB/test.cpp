 DIR* dir = opendir(path.c_str());
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                std::string filePath = path + "/" + entry->d_name;
                struct stat st;
                if (lstat(filePath.c_str(), &st) == 0) 
                {
                    if (S_ISDIR(st.st_mode)) {
                        deleteFiles(filePath);
                    } else {
                        if (unlink(filePath.c_str()) != 0) {
                            std::cerr << "Failed to delete file: " << filePath << std::endl;
                        }
                    }
                }
            }
        }
        closedir(dir);
    }