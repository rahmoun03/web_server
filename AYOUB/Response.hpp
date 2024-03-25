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
		void 	serv_file(map_iterator &type, int &fd, Request &req);
		void 	serv_dir(int &fd, Request &req, Conf &server);
		void	generateResponse(int &fd, Request &req, uint32_t &event, Conf &server);
		void	checkHeaders(Request &req, Conf &server);
		std::string extension(const std::string &path);
		
		
		/*   -       ERRORs        -      */
		
			std::string badRequest();//      				400
			std::string forbidden();//       				403
			std::string notFound(); //      	 			404
			std::string notAllow(std::string method); // 	405
			std::string timeOut(std::string path);//		408
			std::string EntityTooLarge(); // 				413
			std::string longRequest();//     				414
			std::string notImplement(); //  				501
			std::string httpVersion();//    	 			505
		
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
