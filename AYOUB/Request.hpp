/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arahmoun <arahmoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 09:53:30 by arahmoun          #+#    #+#             */
/*   Updated: 2024/02/17 13:39:07 by arahmoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <sstream>
#include <iostream>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>


class Request
{
	private:
		std::stringstream ss;
		std::string method;
        std::string path;
        std::string protocol;
	public:
		Request();
		Request(char *str);
		std::string get_path() const;
		std::string get_method() const;
		std::string get_protocol() const;
		char *req_headers(int fd);
		~Request();
};
char	*ft_strjoin(char *s1, char *s2);


#endif