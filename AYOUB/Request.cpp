/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arahmoun <arahmoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:26:36 by arahmoun          #+#    #+#             */
/*   Updated: 2024/02/24 11:29:08 by arahmoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
}

Request::Request(int fd)
{
	char buffer[1024];  //data buffer of 1K
	ssize_t a;

	// TODO read request
	a = read(fd, buffer, sizeof(buffer));
	while (a > 0 && a <= (ssize_t)sizeof(buffer))
	{
		std::cout <<"read size : " << a << std::endl;
		buffer[a] = '\0';
		ss << buffer;
		a = read(fd, buffer, sizeof(buffer));
	}
	if (a == -1)
	{
		std::cerr << "failure in read request !" << std::endl;
		exit(0);
	}

	std::cout << "request : \n" << ss.str() << std::endl;
	ss >> method;
	ss >> path;
	ss >> protocol;
	path.insert(path.begin(), '.');
}

std::string Request::get_path() const
{
	return path;
}

std::string Request::get_method() const
{
	return method;
}

std::string Request::get_protocol() const
{
	return protocol;
}


char *Request::req_headers(int fd)
{
	char *request = strdup("");
	char *dst = (char *)malloc(1023 + 1);
	ssize_t valread = 1;
	while (valread > 0)
	{
		valread = read(fd, dst, 1023);
		printf("size = %zu\n", valread);
		dst[valread] = '\0';
		request = ft_strjoin(request, dst);
	}
	printf("finish  ...\n");
	if (valread < 0) {
		perror("request read error ");
		exit(EXIT_FAILURE);
	}
	free(dst);
	printf("Requist :\n%s\n", request);
	return request;
}

Request::~Request()
{
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i[3];
	char	*re;

	if (!s1 && !s2)
		return (NULL);
	i[1] = 0;
	i[0] = strlen(s1) + strlen(s2);
	re = (char *)malloc(i[0] + 1);
	if (!re)
		return (NULL);
	while (i[1] <= i[0] && s1[i[1]])
	{
		re[i[1]] = s1[i[1]];
		i[1]++;
	}
	i[2] = 0;
	while (s2[i[2]])
		re[i[1]++] = s2[i[2]++];
	re[i[1]] = '\0';
	free(s1);
		printf("strjoin\n");
	return (re);
}