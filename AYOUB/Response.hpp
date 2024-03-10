#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#define SUPORT_UPLOAD 0
#define UPLOAD_PATH "/nfs/sgoinfre/goinfre/Perso/arahmoun/upload/"

#include "Request.hpp"
typedef std::map<std::string , std::string>::iterator map_iterator;
class Response
{
	private:
		std::ofstream out;
		std::string chunked;
        std::string tmp;
	public:
		Response();
		~Response();

		/*             METHODs              */
		void	GET(int &fd, Request *req);
		void	POST(int &fd, Request *req);
		void	DELETE(int &fd, Request *req);
		/**************************************/

		void	clear();
		void 	serv_file(map_iterator &type, int &fd, Request *req);
		void 	serv_dir(int &fd, Request *req);
		void	imageFile(int &fd, Request *req);
		void	generateResponse(int &fd, Request *req);
		void	checkHeaders(Request *req);
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
		
		
		std::string getResource(std::ifstream &file, std::string &type);
		std::string getImage(std::ifstream &file, const char *type, std::string ext);
};
bool directoryExists(std::string path);
bool fileExists(std::string path);
std::string getCurrentDateTime();

std::map<std::string, std::string> mimeTypes();
std::map<std::string, std::string> ErrorAssets();


#endif