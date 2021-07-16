/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/11 16:28:56 by mchardin          #+#    #+#             */
/*   Updated: 2021/07/02 15:53:14 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request():
	_method(0),
	_path(),
	_uri(),
	_version(),
	_headers(),
	_header_lock(false),
	_body(),
	_body_lock()
{}

Request::Request(Request const &rhs)
{
	*this = rhs;
}

Request::~Request() {}

Request const &
Request::operator=(Request const &rhs)
{
	_method = rhs.get_method();
	_path = rhs.get_path();
	_uri = rhs._uri;
	_version = rhs.get_version();
	_headers = rhs._headers;
	_header_lock = rhs._header_lock;
	_body = rhs._body;
	_body_lock = rhs._body_lock;
	return (*this);
}

AMethod *
Request::get_method() const
{ return (_method); }

std::string const &
Request::get_path() const
{ return (_path); }

std::string const &
Request::get_version() const
{ return (_version); }

std::string const &
Request::get_header(std::string const &key)
{ return (_headers[key]); }

bool
Request::get_header_lock() const
{ return (_header_lock);
}

const std::string &
Request::get_body() const
{ return (_body); }

bool
Request::get_body_lock() const
{ return(_body_lock); }

void
Request::set_method(AMethod * rhs)
{ _method = rhs; }

void
Request::set_path(std::string const &rhs)
{ 
	_path = rhs;
}

void
Request::set_uri(std::string const & uri)
{
	_uri.set_uri(uri);
}

void
Request::set_version(std::string const &rhs)
{ _version = rhs; }

void
Request::set_headers(std::map<std::string, std::string> const &rhs)
{ _headers = rhs; }

void
Request::add_header(std::pair<std::string, std::string> const &rhs)
{
	if (!_header_lock)
		_headers.insert(rhs);
}

void
Request::set_header_lock(bool lock)
{ _header_lock = lock; }

void
Request::set_body(const std::string & body)
{
	if (!_body_lock)
		_body= body;
}

void
Request::set_body_lock(bool lock)
{ _body_lock = lock; }

void
Request::lock_header()
{ set_header_lock(true); }

void
Request::lock_body()
{ set_body_lock(true); }

void
Request::action(Response & response)
{
	if (!_method) //if method == NULL
		throw NoMethod();
	_method->action(*this, response);
}

void
Request::reset()
{
	_method = 0;
	_path = "";
	_version = "";
	_headers.clear();
}

//void
//Request::print_debug() const //TMP
//{
//	cerr << "METHOD CODE : " << _method << " PATH : \"" << _path << "\" " << _version << endl;
//    cerr << "HEADERS : " << endl;
//
//    for (auto& it: _headers)
//        cout << "\"" << it.first << "\" - \"" << it.second << "\"" << endl;
//
//}
