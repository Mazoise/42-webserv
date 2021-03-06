/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:34:55 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 21:19:58 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Upload.hpp"

Upload::Upload(AReaderFileDescriptor * reader) :
_reader(reader),
_state(FIND_BOUNDARY),
_boundary(),
_buffer(),
_position(0)
{}

Upload::Upload(Upload const & src) :
_reader(src._reader)
{
	*this = src;
}

Upload::~Upload(void)
{}

Upload &
Upload::operator=(Upload const & rhs)
{
	if (this != &rhs)
	{
		_reader = rhs._reader;
		_state = rhs._state;
		_boundary = rhs._boundary;
		_buffer = rhs._buffer;
		_position = rhs._position;
		_filename = rhs._filename;
		_message = rhs._message;
	}
	return *this;
}

Upload *
Upload::set_reader(AReaderFileDescriptor & reader)
{
	_reader = &reader;
	return (this);
}

void
Upload::upload(Server & server, const Request & request)
{
	if(_reader == 0)
		throw InvalidReader();
	if (_boundary == "")
	{
		set_boundary(request);
	}
	_buffer = _reader->get_buffer();
	switch (_state)
	{
	case FIND_BOUNDARY:
		find();
		break;
	case HEADER:
		header(server);
		break;
	case WRITE:
		write();
		break;
	default:
		break;
	}
}

void
Upload::set_boundary(const Request & request)
{
	std::string	content_type	= request.get_header("Content-type");
	size_t		p_bound			= content_type.find("boundary");
	size_t		p_egal;

	if (p_bound != std::string::npos)
	{
		p_egal = content_type.find('=', p_bound);
		_boundary = "--" + content_type.substr(p_egal + 1);
	}
}

void
Upload::set_filename(const Message & message)
{
	std::string	header;
	size_t		position;

	if (!message.has_header("Content-Disposition"))
		throw std::exception();
	header = message.get_header("Content-Disposition");
	position = header.find("filename");
	if (position != std::string::npos)
		_filename = StringPP::extract_between(header.substr(position), "\"");
	else
		_filename = "unknow";
}

Upload::e_state
Upload::get_state() const
{
	return (_state);
}

void
Upload::next_position()
{
	if(_reader == 0)
		throw InvalidReader();
	_reader->move_buffer_until(_position);
	_buffer = _reader->get_buffer();
}

bool
Upload::has_boundary()
{
	return ((_position = _buffer.find(_boundary)) != std::string::npos);
}

void
Upload::header(const Server & server)
{
	size_t			pos = 0;
	std::string &	chunck	= _reader->get_chunck();
	std::string		line;

	if (_reader->has_all_headers())
	{
		_reader->cut_header();
		while (chunck.find("\r\n\r\n") != std::string::npos)
		{
			line = chunck.substr(0, chunck.find("\r\n")) + "\r";
			pos = line.find(": ");
			if (pos == std::string::npos)
				throw BadMessage();
			_message.add_header(line.substr(0, pos), line.substr(pos + 2, line.find('\n') - (pos + 2)));
			chunck = chunck.substr(chunck.find("\r\n") + 2);
			if (_message.has_header("Content-Type"))
			{
				set_filename(_message);
				_file.open((server.get_root() + "/" + server.get_upload_path() + "/" + _filename).c_str(), std::fstream::out | std::fstream::trunc);
				if ((_file.rdstate() & std::ifstream::failbit ) != 0)
					throw std::exception();
				_state = WRITE;
			}
			else
				_state = FIND_BOUNDARY;
		}
	}
	else
	{
		_reader->concatenation();
	}
}

void
Upload::find()
{
	if (has_boundary())
	{
		if (_buffer.find(_boundary + "--") == _position)
			_state = END;
		else
		{
			_position += _boundary.length() + 2;
			next_position();
			_state = HEADER;
			_reader->reset_chunck();
		}
	}
}

void
Upload::write()
{
	if (has_boundary())
	{
		_file << _buffer.substr(0, _position - 2);
		_file.close();
		_filename = "";
		_message.clear_header();
		_state = FIND_BOUNDARY;
	}
	else
	{
		_position = _buffer.size() - _boundary.size();
		_file << _buffer.substr(0, _position);
		next_position();
	}
	
}
