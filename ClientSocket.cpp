/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:14:07 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/17 18:14:08 by syndraum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(void) : _socket(0)
{
	// _address = {0};
}

ClientSocket::ClientSocket(ClientSocket const & src)
{
	*this = src;
}

ClientSocket::~ClientSocket(void)
{
	
}

ClientSocket &	ClientSocket::operator=(ClientSocket const & rhs)
{
	if (&rhs != this)
		this->_socket = rhs._socket;
	return *this;
}

int			ClientSocket::getSocket(void) const
{
	return this->_socket;
}

void		ClientSocket::setSocket(int Socket)
{
	this->_socket = Socket;
}

sockaddr	& ClientSocket::getAddress(void)
{
	return this->_address;
}

void		ClientSocket::setAddress(sockaddr & Address)
{
	this->_address = Address;
}