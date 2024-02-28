/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arahmoun <arahmoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:26:36 by arahmoun          #+#    #+#             */
/*   Updated: 2024/02/28 12:29:10 by arahmoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
}

Request::Request(std::stringstream &buf)
{
		ss << buf.str();
		std::string key;
		char buf;
		std::string value;
		std::string body;
		
		// std::cout << "request : \n"
		// 		  << ss.str() << std::endl;
		ss >> method;
		ss >> path;
		ss >> protocol;
		path = SERVER_ROOT + path;
		while (ss)
		{
			ss >> key;
			std::getline(ss, value);
			headers[key] = value;
			if(key == "Content-Length:")
			{
				std::cout << "there is a body \n";
				while (ss)
				{
					ss >> buf;
					body.push_back(buf);
				}
				break;
			}
		}
		// std::cout << "\n--------------------------------------------------------\n" <<std::endl;
}

std::ostream &operator<<(std::ostream &os, const Request &other)
{
	os << other.get_method() << " ";
	os << other.get_path() << " ";
	os << other.get_protocol() << "\n";
	std::map<std::string, std::string>::const_iterator it = other.get_headers().begin();
	std::map<std::string, std::string>::const_iterator ite = other.get_headers().end();
	for (; it != ite; ++it)
		os << it->first <<BLUE<< "|" <<YOLLOW << it->second << '\n';
	os << "\n\nbody :\n" << other.get_body() << '\n';
	return os;
}

const std::map<std::string, std::string> &Request::get_headers() const
{
	return headers;
}

const std::string Request::get_body() const
{
	return body;
}


const std::string Request::get_path() const
{
	return path;
}

const std::string Request::get_method() const
{
	return method;
}

const std::string Request::get_protocol() const
{
	return protocol;
}

Request::~Request()
{
}
