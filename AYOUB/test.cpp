int isInside(const std::string& dpath, const std::string& root) 
{
    std::string root_ = root;
    if (root_[root_.length() - 1] != '/' && root_[root_.length() - 1] != '\\') 
    {
        root_ += '/';
    }
    std::cout << root_ << std::endl;
    // exit(1);
    return dpath.find(root_) == 0;
}




int	Response::DELETE(int &fd, Request &req, Conf &server, std::string dpath)
{
    std::string root =  server.locat.find(req.locationPath)->second.root;
    const char* path = root.c_str();
    const char* path2 = dpath.c_str();

    char resolved_path[PATH_MAX];
    std::stringstream ss;
    std::stringstream ss1;
    ss  << realpath(path, resolved_path);
    ss1 << realpath(path2, resolved_path);
    std::string str1;
    std::string str2;
    ss >> str1;
    ss1 >> str2;
    if(isInside(str1, str2))   
        throw forbidden();