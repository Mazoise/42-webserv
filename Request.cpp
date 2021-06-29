/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:28:56 by mchardin          #+#    #+#             */
/*   Updated: 2021/06/25 15:29:22 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{}


Request::Request(Request const &rhs)
{
	*this = rhs;
}

Request::~Request()
{}

Request const 				&Request::operator=(Request const &rhs)
{
	_method = rhs.get_method();
	_path = rhs.get_path();
	_version = rhs.get_version();
	_headers = rhs.get_headers();
	return (*this);
}

AMethod *						Request::get_method() const { return (_method); }
std::string const				&Request::get_path() const { return (_path); }
std::string const				&Request::get_version() const { return (_version); }
std::map<std::string, std::string> const	&Request::get_headers() const { return (_headers); }
std::string const				&Request::get_header(std::string const &key) { return (_headers[key]); }

void						Request::set_method(AMethod * rhs){ _method = rhs; }
void						Request::set_path(std::string const &rhs){ _path = rhs; }
void						Request::set_version(std::string const &rhs){ _version = rhs; }
void						Request::set_headers(std::map<std::string, std::string> const &rhs){ _headers = rhs; }
void						Request::add_header(std::pair<std::string, std::string> const &rhs){ _headers.insert(rhs); }
void						Request::action(Response & response)
{
		_method->action(*this, response);
}

//void						Request::print_debug() const //TMP
//{
//	cerr << "METHOD CODE : " << _method << " PATH : \"" << _path << "\" " << _version << endl;
//    cerr << "HEADERS : " << endl;
//
//    for (auto& it: _headers)
//        cout << "\"" << it.first << "\" - \"" << it.second << "\"" << endl;
//
//}
