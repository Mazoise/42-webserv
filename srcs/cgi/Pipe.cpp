/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipe.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:33:38 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/18 14:33:39 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pipe.hpp"

Pipe::Pipe(void) :
_out(2, 0),
_err(2, 0),
_in(2, 0)
{}

Pipe::Pipe(Pipe const & src)
{
	*this = src;
}

Pipe::~Pipe(void)
{
	
}

Pipe &	Pipe::operator=(Pipe const & rhs)
{
	if (this != &rhs)
	{
		_out = rhs._out;
		_err = rhs._err;
		_in = rhs._in;
	}
	return *this;
}

int *
Pipe::get_out()
{
	return (&_out.front());
}

int *
Pipe::get_err()
{
	return (&_err.front());
}

int *
Pipe::get_in()
{
	return (&_in.front());
}