std::string normalizePath(const std::string& path) 
{
    std::string result;
    const char* delim = "/";
    char* token = std::strtok(const_cast<char*>(path.c_str()), delim);
    while (token != nullptr) 
    {
        if (strcmp(token, "..") == 0) 
        {
            if (!result.empty()) 
            {
                size_t pos = result.find_last_of("/");
                if (pos != std::string::npos)
                    result.erase(pos);
            }
        } 
        else if (strcmp(token, ".") != 0) 
        {
            result += "/";
            result += token;
        }
        token = std::strtok(nullptr, delim);
    }
    return result;
}

bool isPathOutside(const std::string& pathA, const std::string& pathB) {
    std::string normalizedPathA = normalizePath(pathA);
    std::string normalizedPathB = normalizePath(pathB);

    return normalizedPathA.find(normalizedPathB) != 0;
}