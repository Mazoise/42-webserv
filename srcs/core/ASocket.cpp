/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:50:07 by cdai              #+#    #+#             */
/*   Updated: 2021/07/24 19:22:09 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"

ASocket::ASocket(void) :
_id(-1),
_reader(1)
{
//	reset_buffer();
}

ASocket::ASocket(ASocket const & rhs):
_reader(rhs._reader)
{
	*this = rhs;
}

ASocket::~ASocket(void)
{}

ASocket const & ASocket::operator=(ASocket const & rhs)
{
	if (this != &rhs)
	{
		_socket = rhs._socket;
		_address = rhs._address;
		_id = rhs._id;
		_reader = rhs._reader;
	}
//	for (int i = 0; i < BUFFER_SIZE; i++)
//		_buffer[i] = rhs._buffer[i];
	return *this;
}

int
ASocket::get_socket(void) const
{
	return _socket;
}

void
ASocket::set_socket(int socket)
{
	_socket = socket;
	_reader.set_fd(socket);
}

const sockaddr_in &
ASocket::get_address(void) const
{
	return _address;
}

void
ASocket::set_address(const sockaddr_in & Address)
{
	this->_address = Address;
}

int
ASocket::get_id() const
{ return (_id); }


void
ASocket::set_id(int id)
{ _id = id; }

ReaderSocket &
ASocket::get_reader(void)
{
	return (_reader);
}