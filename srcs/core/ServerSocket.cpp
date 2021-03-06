/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:14:23 by syndraum          #+#    #+#             */
/*   Updated: 2021/11/19 21:23:58 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int port) :
_port(port),
_ip("0.0.0.0"),
_active(true)
{
	_socket = -1;
}

ServerSocket::ServerSocket(int port, std::string ip) :
_port(port),
_ip(ip),
_active(true)
{
	_socket = -1;
}

ServerSocket::ServerSocket(ServerSocket const & src)
{
	*this = src;
}

ServerSocket::~ServerSocket(void)
{
	sclose();
}

ServerSocket &
ServerSocket::operator=(ServerSocket const & rhs)
{
	if (&rhs != this){
		this->_port = rhs._port;
		this->_socket = rhs._socket;
		this->_address = rhs._address;
		this->_ip = rhs._ip;
		this->_active = rhs._active;
	}
	return *this;
}

void
ServerSocket::setup_socket()
{
	int one = 1;

	_socket = socket(AF_INET , SOCK_STREAM | SOCK_NONBLOCK , 0);
	if (_socket == -1)
		throw SocketError();
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(_ip.c_str());
	_address.sin_port = htons( _port );
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(int));
}

void
ServerSocket::bind_socket()
{
	if ((bind(_socket, reinterpret_cast<sockaddr*>(&_address), sizeof(_address)) < 0))
		throw BindError();
	std::cout << "Bind done. Listen on port " << _port <<std::endl;
}

void
ServerSocket::listen_socket(int worker_connection)
{
	if ((listen(_socket, worker_connection)) < 0)
		throw ListenError();
	std::cout << "Waiting for incoming connection..." << std::endl;
}

ServerSocket *
ServerSocket::set_port(int port)
{
	_port = port;
	return this;
}

int
ServerSocket::get_port() const
{
	return (_port);
}

const std::string &
ServerSocket::get_ip() const
{
	return (_ip);
}

ServerSocket *
ServerSocket::set_active(bool active)
{
	_active = active;
	return this;
}

bool
ServerSocket::get_active() const
{
	return (_active);
}
