/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestCGI.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdai <cdai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 15:14:02 by cdai              #+#    #+#             */
/*   Updated: 2021/07/25 17:59:20 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestCGI.hpp"

RequestCGI::RequestCGI(CGI & cgi):
_cgi(cgi)
{}

RequestCGI::~RequestCGI(void)
{}

RequestCGI const &
RequestCGI::operator=(RequestCGI const &rhs)
{
	_cgi = rhs._cgi;
	return (*this);
}

Message * RequestCGI::send(const std::string & script_path, AReaderFileDescriptor & reader)
{
	return (_cgi.start(*this, script_path, reader));
}
