/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arahmoun <arahmoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:26:36 by arahmoun          #+#    #+#             */
/*   Updated: 2024/02/25 16:25:26 by arahmoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
}

Request::Request(int &fd)
{
	char buffer[1024]; // data buffer of 1K
	ssize_t a;

	// TODO read request
	if ((a = recv(fd, buffer, sizeof(buffer), 0)) < 0)
	{
		std::cerr << "failure in read request !" << std::endl;
	}
	else
	{
		std::string key;
		std::string value;
		buffer[a] = '\0';
		ss << buffer;
		std::cout << "request : "<<a<<"bytes\n"
				  << ss.str() << std::endl;
		ss >> method;
		ss >> path;
		ss >> protocol;
		path.insert(path.begin(), '.');
		while (ss)
		{
			ss >> key;
			std::getline(ss, value);
			headers[key] = value;
		}
		// std::cout << "\n--------------------------------------------------------\n" <<std::endl;
	}
}

std::ostream &operator<<(std::ostream &os, const Request &other)
{
	os << other.get_method() << " ";
	os << other.get_path() << " ";
	os << other.get_protocol() << "\n\n";
	std::map<std::string, std::string>::const_iterator it = other.get_headers().begin();
	std::map<std::string, std::string>::const_iterator ite = other.get_headers().end();
	for (; it != ite; ++it)
		os << it->first << it->second << '\n';
	return os;
}

const std::map<std::string, std::string> &Request::get_headers() const
{
	return headers;
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
