/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdai <cdai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:50:07 by cdai              #+#    #+#             */
/*   Updated: 2021/06/28 19:11:30 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"

ASocket::~ASocket(void)
{}

int	ASocket::get_socket(void) const
{
	return _socket;
}

void	ASocket::set_socket(int socket)
{
	_socket = socket;
}

int		ASocket::get_next_line(std::string & str)
{
	int	response = 2;
	int ret = 1;
	static char buffer[BUFFER_SIZE];

	std::string temp("");
	size_t found;
	str = "";

	while (response > 1)
	{
		temp += buffer;
		found = temp.find("\r\n");
//		std::cout << "temp: " << temp << std::endl;
//		std::cout << "found: " << found << std::endl;
		if (ret == 0)
		{
			str = temp;
			response = 0;
			break;
		}
		else if (found == std::string::npos)
		{
			ret = read(_socket, buffer, BUFFER_SIZE - 1);
			buffer[ret] = 0;
		}
		else
		{
			// strdup ou substr
			temp.copy(buffer, found, 0);
			buffer[found] = 0;
			str = buffer;

			response = 1;
			break;
		}
	}


	if (temp.length() > found)
	{
		temp.copy(buffer, temp.length() - found - 2, found + 2);
		buffer[temp.length() - found - 2] = 0;
	}

	return response;
}
