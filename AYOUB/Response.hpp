#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
typedef std::map<std::string , std::string>::iterator map_iterator;
class Response
{
	private:

	public:
		Response();
		~Response();
		
		
		// void	GET();
		// void	POST();
		// void	DELETE();

		// void clear();
		void 	serv_file(map_iterator &type, int &fd, Request *req);
		void 	serv_dir(int &fd, Request *req);
		void	imageFile(int &fd, Request *req);
		void	generateResponse(int &fd, Request *req);
		void	checkHeaders(Request *req);
		std::string extension(const std::string &path);
		std::string notFound();
		std::string notImplement();
		std::string badRequest();
		std::string EntityTooLarge();
		std::string longRequest();
		std::string homepage();
		std::string getResource(std::ifstream &file, std::string &type);
		std::string getImage(std::ifstream &file, const char *type, std::string ext);
};
bool directoryExists(std::string path);
bool fileExists(std::string path);

std::map<std::string, std::string> mimeTypes();
std::map<std::string, std::string> ErrorAssets();

// void Response::clear()
// {
//     // startline.str("");
// 	// method.clear();
// 	// path.clear();
// 	// protocol.clear();
// 	// headers.clear();
// 	// body.str("");
// }

#endif