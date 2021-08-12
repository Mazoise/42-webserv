/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore_utils.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 17:01:50 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/10 17:01:51 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BuilderCore.hpp"

void
BuilderCore::erase_server_bool()
{
	_b_server_name = false; // erase?
	_b_server_root = false;
	_b_server_path_error_page = false;
	_b_server_auto_index = false;
	_b_server_client_max_body_size = false;
}

std::string
BuilderCore::next_word_skip()
{
	skip_whitespaces();
	if (_idx >= _line.length())
		return("");
	size_t			len = _line.find_first_of(";{}# \n\r\t\v\f", _idx) - _idx;

	if (_line[_idx] == '\"' || _line[_idx] == '\'')
	{
		_idx++;
		len = _line.find_first_of("\"\'", _idx) - _idx;
	}
	if (len == std::string::npos)
		unexpected_eof_error("\";\" or \"}\"");
	std::string		ret = _line.substr(_idx, len);
	_idx += len;
	if (_line[_idx] == '\"' || _line[_idx] == '\'')
		_idx++;
	return(ret);
}

int
BuilderCore::line_count()
{
	std::string::iterator stop = _line.begin() + _idx;
	if (_idx >= _line.length())
		stop = _line.end();
	return (std::count(_line.begin(), stop, '\n') + 1);
}

int
BuilderCore::skip_whitespaces()
{
	std::locale	loc;
	int			count = _idx;

	while (_idx < _line.length())
	{
		if (_line[_idx] ==  '#')
			skip_comments();
		if (!std::isspace(_line[_idx], loc))
			return (_idx - count);
		_idx++;
	}
	return (_idx - count);
}

void
BuilderCore::skip_comments()
{
	size_t tmp;

	while (_line[_idx] == '#')
	{
		tmp = _line.find('\n', _idx);
		if (tmp != std::string::npos)
			_idx = tmp + 1;
		else
			_idx = _line.length();
		skip_whitespaces();
	}
}

int
BuilderCore::stoi_skip()
{
	int			ret;
	int			i = 0;
	std::locale loc;

	while(_line[i + _idx])
	{
		if (!isdigit(_line[i + _idx], loc))
			break ;
		i++;
	}
	if (!_line[i + _idx])
		unexpected_eof_error("\";\" or \"}\"");
	else if (!i)
		return (-1);
	std::stringstream ss(_line.substr(_idx, i));
	ss >> ret;
	_idx += i;
	return (ret);
}

std::string
BuilderCore::check_return_ip(int first_number, int cursor)
{
	int			i = 2;
	int			tmp;
	std::string	ret;

	if (first_number < 0 || first_number > 255)
	{
		_idx = cursor;
		host_not_found_error(next_word_skip());
	}
	_idx++;
	while (i >= 0)
	{
		tmp = stoi_skip();
		if (tmp < 0 || tmp > 255 || (_line[_idx] != '.' && i != 0))
		{
			_idx = cursor;
			host_not_found_error(next_word_skip());
		}
		i--;
		_idx++;
	}
	return(_line.substr(cursor, _idx - cursor - 1));
}

void
BuilderCore::check_semicolon(std::string directive)
{
	skip_whitespaces();
	if (!_line[_idx])
		unexpected_eof_error("\";\" or \"}\"");
	else if (_line[_idx] == '}')
		unexpected_character_error('}');
	else if (_line[_idx] != ';')
		not_terminated_by_semicolon_error(directive);
	_idx++;
}
