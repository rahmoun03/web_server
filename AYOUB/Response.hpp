#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"

class Response
{
	private:
		std::ofstream out;
	public:
		Response();
		~Response();
		
		
		// void	GET();
		// void	POST();
		// void	DELETE();

		// void clear();
		void	htmlFile(int &fd, Request &req);
		void	imageFile(int &fd, Request &req);
		void	generateResponse(int &fd, Request &req);
		std::string extension(const std::string &path);
		std::string notFound();
		std::string homepage();
		std::string getResource(std::ifstream &file, const char *type, std::string ext);
		std::string getImage(std::ifstream &file, const char *type, std::string ext);
};

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