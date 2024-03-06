#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#define IMG_ASSET "./assets"
#include "Request.hpp"
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
		void	htmlFile(int &fd, Request *req);
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
		std::string getResource(std::ifstream &file, const char *type, std::string ext);
		std::string getImage(std::ifstream &file, const char *type, std::string ext);
};
bool directoryExists(const char* path);
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