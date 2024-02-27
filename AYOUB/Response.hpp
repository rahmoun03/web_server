#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"

class Response
{
	private:
		/* data */
	public:
		Response();
		Response(int &fd, Request *req);
		void	htmlFile(int &fd, Request *req);
		void	imageFile(int &fd, Request *req);
		// void	GET();
		// void	POST();
		// void	DELETE();
		std::string extension(const std::string &path);
		std::string notFound();
		std::string homepage();
		std::string getResource(std::ifstream &file, const char *type, std::string ext);
		std::string getImage(std::ifstream &file, const char *type, std::string ext);
		~Response();
};

#endif