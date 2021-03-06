/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReaderSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 11:45:01 by cdai              #+#    #+#             */
/*   Updated: 2021/11/18 14:50:41 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReaderSocket.hpp"

ReaderSocket::ReaderSocket(int fd):
AReaderFileDescriptor(fd)
{}

ReaderSocket::ReaderSocket(ReaderSocket const & rhs)
{
	*this = rhs;
}

ReaderSocket::~ReaderSocket(void)
{}

ReaderSocket const & ReaderSocket::operator=(ReaderSocket const & rhs)
{
	if (&rhs != this)
	{
		AReaderFileDescriptor::operator=(rhs);
	}
	return (*this);
}

int
ReaderSocket::_read(void * address, int n_read)
{
	return (recv(_fd, address, n_read, 0));
}
