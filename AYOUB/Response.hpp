#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "../CHEBCHOUB/conf.hpp"
#include <filesystem>

// class Conf;
// class netPlix;
class netPlix;
class Conf;
#include "Request.hpp"
typedef std::map<std::string , std::string>::iterator map_iterator;
class Conf;
class Response
{
	private:
		std::ofstream out;
		std::string chunked;
		size_t decimal;
		std::string str;
        std::string tmp;
		std::string path;
	public:
		int file;
		Response();
		~Response();

		/*             METHODs              */
		void	GET(int &fd, Request &req, Conf &server);
		void	POST(int &fd, Request &req, Conf &server);
		int	DELETE(int &fd, Request &req, Conf &server, std::string dpath);
		/**************************************/

		void	clear();
		void 	serv_file(map_iterator &type, int &fd, Request &req, Conf &server);
		void 	serv_dir(int &fd, Request &req, Conf &server);
		int 	serveCgi(Request &req);

		void	generateResponse(int &fd, Request &req, Conf &server);
		void	checkHeaders(Request &req, Conf &server);
		std::string extension(const std::string &path);
		
		
		/*   -       4XX        -      */
		
			std::string badRequest(std::string path);//      				400
			std::string forbidden(std::string path);//       				403
			std::string notFound(std::string path, Request &req); //      	 			404
			std::string notAllow(std::string method, std::string path); // 	405
			std::string timeOut(std::string path);//						408
			std::string lengthRequired(std::string path);// 				411	
			std::string EntityTooLarge(std::string path); // 				413
			std::string longRequest(std::string path);//     				414
			std::string mediaType(std::string path);//						415
			std::string headerTooLarge(std::string path); // 				431

		
		
		/*            5XX               */
			std::string serverError(std::string path);// 					500
			std::string notImplement(std::string path); //  				501
			std::string httpVersion(std::string path);//    	 			505

			// std::string homepage(); // 200 home
		/***********************/
		
		void Redirect(std::string &location, Request &req, int &fd);
		
		std::string getResource(int &file, Request &req);
		std::string getRedirctionS(std::string &location);
};
std::string listDirectory(const char* path);
bool directoryExists(std::string path);
bool fileExists(std::string path);
std::string getCurrentDateTime();

std::map<std::string, std::string> mimeTypes();


#endif
