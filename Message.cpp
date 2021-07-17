#include "Message.hpp"

Message::Message(void) :
	_headers(),
	_header_lock(false),
	_body(),
	_body_lock(false)
{}

Message::Message(Message const & src)
{
	*this = src;
}

Message::~Message(void)
{
	
}

Message &	Message::operator=(Message const & rhs)
{
	_headers = rhs._headers;
	_header_lock = rhs._header_lock;
	_body = rhs._body;
	_body_lock = rhs._body_lock;
	return *this;
}

std::string const &
Message::get_header(std::string const &key)
{ return (_headers[key]); }

bool
Message::get_header_lock() const
{ return (_header_lock);
}

const std::string &
Message::get_body() const
{ return (_body); }

bool
Message::get_body_lock() const
{ return(_body_lock); }

Message &
Message::add_header(const std::string & name, const std::string & content)
{
	_headers.insert(std::pair<std::string, std::string>(name, content));
	return *this;
}

// void
// Message::add_header(std::pair<std::string, std::string> const &rhs)
// {
// 	if (!_header_lock)
// 		_headers.insert(rhs);
// }

void
Message::set_header_lock(bool lock)
{ _header_lock = lock; }

void
Message::set_body(const std::string & body)
{
	if (!_body_lock)
		_body= body;
}

void
Message::set_body_lock(bool lock)
{ _body_lock = lock; }

void
Message::lock_header()
{ set_header_lock(true); }

void
Message::lock_body()
{ set_body_lock(true); }
