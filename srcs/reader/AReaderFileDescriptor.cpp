/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AReaderFileDescriptor.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 11:22:22 by cdai              #+#    #+#             */
/*   Updated: 2021/11/19 21:06:49 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AReaderFileDescriptor.hpp"

AReaderFileDescriptor::AReaderFileDescriptor(void)
{}

AReaderFileDescriptor::AReaderFileDescriptor(int fd):
_fd(fd),
_size(0),
_chunck(),
_account_body(0),
_limit_body(0)
{
	_reset_buffer();
}

AReaderFileDescriptor::AReaderFileDescriptor(AReaderFileDescriptor const & rhs)
{
	*this = rhs;
}

AReaderFileDescriptor::~AReaderFileDescriptor(void)
{}

AReaderFileDescriptor const &
AReaderFileDescriptor::operator=(AReaderFileDescriptor const & rhs)
{
	if (this != &rhs)
	{
		_fd = rhs._fd;
		for (int i = 0; i < BUFFER_SIZE; i++)
			_buffer[i] = rhs._buffer[i];
		_size = rhs._size;
		_chunck = rhs._chunck;
		_account_body = rhs._account_body;
		_limit_body = rhs._limit_body;
	}
	return *this;
}

void
AReaderFileDescriptor::_reset_buffer(void)
{
	for (int i = 0; i < BUFFER_SIZE; i++)
		_buffer[i] = '\0';
	_size = 0;
}

std::string
AReaderFileDescriptor::get_buffer(void) const
{
	std::string tmp = std::string(_buffer, _size);
	return (tmp);
}

int
AReaderFileDescriptor::get_fd(void)
{
	return (_fd);
}

void
AReaderFileDescriptor::set_fd(int fd)
{
	_fd = fd;
}

int
AReaderFileDescriptor::next_read(size_t start)
{
	std::string	tmp;
	size_t		remain	= 0;
	int			ret		= 1;

	if (start > BUFFER_SIZE - 1)
		throw OutOfBuffer();
	if (start != BUFFER_SIZE - 1){
		if (start >= _size)
			throw OutOfBound();
		remain = _size - start;
		tmp = std::string(&_buffer[start], remain);
		tmp.copy(_buffer, remain);
		_buffer[remain] = 0;
	}
	ret = _read(&_buffer[remain], BUFFER_SIZE - 1 - remain);
	if (ret == -1)
		throw ReadError();
	_size = remain;
	if (ret >= 0)
		_size += ret;
	if (ret == 0)
		throw EndOfFile();
	return(ret);
}

int
AReaderFileDescriptor::fill_buffer()
{
	int ret = 0;

	if (_size >= BUFFER_SIZE - 1)
		return (-2);
	ret = _read(&_buffer[_size], BUFFER_SIZE - 1 - _size);
	if (ret == -1)
		throw ReadError();
	else if (ret == 0)
		throw EndOfFile();
	if (ret >= 0)
		_size += ret;
	_account_body += ret;
	return (ret);
}

int
AReaderFileDescriptor::write_body(int fd)
{
	ssize_t ret = 0;

	if (_size == 0)
		return (ret);
	ret = write (fd, _buffer, _size);
	if (ret == -1)
		throw WriteError();
	move_buffer_until(ret);
	return(ret);
}

bool
AReaderFileDescriptor::has_line() const
{
	if (_chunck.find("\r\n") == std::string::npos)
		return (false);
	return (true);
}



bool
AReaderFileDescriptor::has_all_headers() const
{
	if (_chunck.find("\r\n\r\n") == std::string::npos)
		return (false);
	return (true);
}

void
AReaderFileDescriptor::concatenation()
{
	std::string tmp(_buffer, _size);

	_chunck = _chunck + tmp;
}

void
AReaderFileDescriptor::reset_chunck()
{
	_chunck = "";
}

void
AReaderFileDescriptor::cut_header()
{
	size_t		pos_buffer;
	size_t		pos_end_of_header;
	size_t		pivot;
	std::string	tmp;

	pos_buffer = _chunck.find(_buffer);
	if (pos_buffer == std::string::npos)
		return;
	pos_end_of_header = _chunck.find("\r\n\r\n") + 4;
	if (pos_end_of_header == std::string::npos)
		return;
	pivot = pos_end_of_header - pos_buffer;
	if (pivot != 0)
	{
		tmp = std::string(_buffer, _size);
		tmp.copy(_buffer, _size - pivot, pivot);
		_buffer[_size - pivot] = 0;
		_size = _size - pivot;
	}
}

void
AReaderFileDescriptor::move_buffer_until(size_t pos)
{
	std::string	tmp;
	size_t		remain	= 0;

	if (pos > _size)
		throw OutOfBound();
	if (pos == 0)
		return ;
	remain = _size - pos;
	tmp = std::string(&_buffer[pos], remain);
	tmp.copy(_buffer, remain);
	_buffer[remain] = 0;
	_size = remain;
}

std::string &
AReaderFileDescriptor::get_chunck()
{
	return (_chunck);
}

size_t
AReaderFileDescriptor::get_account_body() const
{
	return (_account_body);
}

void
AReaderFileDescriptor::set_account_body(size_t account)
{
	_account_body = account;
}

size_t
AReaderFileDescriptor::get_size() const
{
	return (_size);
}

void
AReaderFileDescriptor::set_limit(size_t limit)
{
	_limit_body = limit;
}

bool
AReaderFileDescriptor::body_full() const
{
	return (_account_body >= _limit_body );
}
