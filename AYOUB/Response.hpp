#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#define SUPORT_UPLOAD 1
// #define UPLOAD_PATH "/nfs/sgoinfre/goinfre/Perso/himejjad/upload/"
#define UPLOAD_PATH "www/server1/upload/"

// class Conf;
// class netPlix;
class netPlix;
class Conf;
#include "Request.hpp"
typedef std::map<std::string , std::string>::iterator map_iterator;
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
		void	GET(int &fd, Request &req);
		void	POST(int &fd, Request &req, std::map<int,Conf> &server, std::vector<int> &socket_acc);
		void	DELETE(int &fd, Request &req);
		/**************************************/

		void	clear();
		void 	serv_file(map_iterator &type, int &fd, Request &req);
		void 	serv_dir(int &fd, Request &req);
		// void	imageFile(int &fd, Request &req);
        // std::map<int,Conf> server;

		void	generateResponse(int &fd, Request &req, uint32_t &event, std::map<int,Conf> &server, std::vector<int> &socket_acc);
		void	checkHeaders(Request &req);
		std::string extension(const std::string &path);
		
		
		/*   -       ERRORs        -      */
		
			std::string badRequest();//      400
			std::string forbidden();//       403
			std::string notFound(); //       404
			std::string EntityTooLarge(); // 413
			std::string longRequest();//     414
			std::string notImplement(); //   501
			std::string httpVersion();//     505
		
			std::string homepage(); // 200 home
		/***********************/
		
		
		std::string getResource(int &file, Request &req);
		std::string getRedirctionS(std::string &type, std::string &location);
		std::string getImage(std::ifstream &file, const char *type, std::string ext);
};
bool directoryExists(std::string path);
bool fileExists(std::string path);
std::string getCurrentDateTime();

std::map<std::string, std::string> mimeTypes(); 
std::map<std::string, std::string> ErrorAssets();


#endif
