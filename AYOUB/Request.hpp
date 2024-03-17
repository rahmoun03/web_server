/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: himejjad <himejjad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 09:53:30 by arahmoun          #+#    #+#             */
/*   Updated: 2024/03/17 02:50:50 by himejjad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <sstream>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <ctime>
#include <bits/stdc++.h> 



#define SERVER_ROOT "./www/server1"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YOLLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RAN "\033[0;38m"
#define DEF "\033[0m"

class Request
{
	private:

		std::stringstream startline;
		std::string method;
		std::string path;
		std::string protocol;
		std::map<std::string, std::string> headers;
		std::stringstream body;

	public:
		int chun;
		size_t ra;
		bool startLineForma;
		long int body_limit;
		bool firstTime;
		bool connexion;
		
		
		Request();
		Request(const Request &other);
		Request &operator=(const Request &other);
		~Request();

		void clear();
		Request(std::stringstream &buf, size_t &endOf);
		std::string &get_path();
		const std::string get_method() const;
		const std::string get_body() const;
		const std::string get_protocol() const;
		const std::string get_header(const char *key);
		const std::map<std::string, std::string> &get_headers() const;
};


std::ostream &operator<<(std::ostream &os, Request &other);
size_t findEndOfHeaders(char* buffer, ssize_t bufferSize);

#endif
