/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:14:07 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/29 11:33:20 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(void)
{
	// _address = {0};
	_socket = 0;
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
