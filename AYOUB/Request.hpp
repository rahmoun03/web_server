/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arahmoun <arahmoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 09:53:30 by arahmoun          #+#    #+#             */
/*   Updated: 2024/02/28 16:10:05 by arahmoun         ###   ########.fr       */
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

#define SERVER_ROOT "./assets"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YOLLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define DEF "\033[0m"

class Request
{
	private:
		std::stringstream ss;
		std::string method;
		std::string path;
		std::string protocol;
		std::map<std::string, std::string> headers;
		std::stringstream body;

	public:
		Request();
		Request(std::stringstream &buf);
		const std::string get_path() const;
		const std::string get_method() const;
		const std::string get_body() const;
		const std::string get_protocol() const;
		const std::map<std::string, std::string> &get_headers() const;
		~Request();
};
std::ostream &operator<<(std::ostream &os, const Request &other);

#endif