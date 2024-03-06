/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbajaou <ahbajaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:26:36 by arahmoun          #+#    #+#             */
/*   Updated: 2024/03/02 16:00:55 by ahbajaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
}

Request::Request(std::stringstream &buf, size_t &endOf)
{
		std::string key;
		std::string dst;
		std::string value;

		std::getline(buf, dst);
		size_t i = dst.size() + 1;
		
		startline << dst;
		startline >> method;
		startline >> path;
		startline >> protocol;
		path = SERVER_ROOT + path;
		
		while(i < endOf && buf >> key && std::getline(buf, value))
		{
			headers[key] = value;
			i += key.length() + value.length() + 1;
		}
		if(buf && (endOf + 4) < buf.str().size())
		{
			buf >> key;
			body << key << buf.rdbuf();
		}
		firstTime = true;
		// buf.str("");
		// std::cout << "\n--------------------------------------------------------\n" <<std::endl;
}

size_t findEndOfHeaders(char* buffer, ssize_t bufferSize)
{
	const char *end = "\r\n\r\n";
	size_t size = strlen(end);
	for (ssize_t i = 0; i < bufferSize; i++)
	{
		if (strncmp(buffer + i, end, size) == 0)
		{
			return i;
		}
	}
	return -1;
}

Request::Request(const Request &other)
{
	*this = other;
}


Request &Request::operator=(const Request &other)
{
	if(this != &other)
	{
		startline << other.startline.str();
		method = other.method;
		path = other.path;
		protocol = other.protocol;
		firstTime = other.firstTime;
		headers = other.headers;
		body << other.body.str();
	}
	return *this;
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
	os << "body :\n" << other.get_body() << RED <<"." << '\n';
	return os;
}

void Request::clear()
{
    startline.str("");
	method.clear();
	path.clear();
	protocol.clear();
	headers.clear();
	body.str("");
	connexion = false;
}

const std::map<std::string, std::string> &Request::get_headers() const
{
	return headers;
}

const std::string Request::get_header(const char *key)
{
    return (headers[key]);
}



const std::string Request::get_body() const
{
	return body.str();
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
