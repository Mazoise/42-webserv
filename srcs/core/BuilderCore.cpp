/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 17:04:45 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/03 23:41:56 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BuilderCore.hpp"

BuilderCore::BuilderCore(std::istream &fd, Core *core) :
_idx(0),
_core(core),
_b_worker(false)
{
	std::string	directive;
	std::getline(fd, _line, char(EOF));
	while (_idx < _line.length())
	{
		directive = next_word_skip();
		if (!directive.compare("worker"))
			parse_worker();
		else if (!directive.compare("server"))
			parse_server(core);
		else if (_line[_idx] ==  ';')
			unexpected_character_error(';');
		else if (_line[_idx] ==  '}')
			unexpected_character_error('}');
		else if (directive != "")
			unknown_directive_error(directive);
	}
}

BuilderCore::~BuilderCore() {}

Core *
BuilderCore::get_builded_core() const
{
	return (_core);
}

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
BuilderCore::parse_server_listen(Server *server)
{
	int			port = 80;
	std::string	ip = "0.0.0.0";
	int			cursor;
	bool		active;

	skip_whitespaces();
	if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("listen");
	while (_line[_idx] && _line[_idx] != ';' && _line[_idx] != '}')
	{
		cursor = _idx;
		port = stoi_skip();
		if (port < 0 || (!skip_whitespaces() && _line[_idx] != ';' && _line[_idx] != '.'))
		{
			_idx = cursor;
			host_not_found_error(next_word_skip());
		}
		if (_line[_idx] == '.')
		{
			ip = check_return_ip(port, cursor);
			if (_line[_idx] == ':')
			{
				port = stoi_skip();
				if (port < 0|| (!skip_whitespaces() && _line[_idx] != ';'))
				{
					_idx = cursor;
					host_not_found_error(next_word_skip());
				}
			}
			else
				port = 8080;
		}
		active = !_core->has_host_port(ip, port);
		server->add_listen(port, ip, active);
		skip_whitespaces();
	}
	check_semicolon("listen");
}

void
BuilderCore::parse_server_index(Server *server)
{
	std::string	index;

	skip_whitespaces();
	if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("index");
	while (_line[_idx] && _line[_idx] != ';' && _line[_idx] != '}')
	{
		index = next_word_skip();
		// std::cerr << "Index : " << index << std::endl; 
		server->add_index(index);
	}
	check_semicolon("index");
}

void
BuilderCore::parse_server_allow_methods(Server *server, Core *core)
{
	std::string	allow_methods;
	AMethod *	tmp;

	skip_whitespaces();
	if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("allow_methods");
	while (_line[_idx] && _line[_idx] != ';' && _line[_idx] != '}')
	{
		allow_methods = next_word_skip();
		tmp = core->get_method(allow_methods);
		if (tmp)
			server->add_method(tmp);
		else
		{
			std::cerr << "Parsing Error : unimplemented method \"" << allow_methods << "\" in the \"allow_methods\" directive on line " << line_count() << std::endl;
			throw(ParsingError());
		}
	}
	check_semicolon("allow_methods");
}

void
BuilderCore::parse_server_name(Server *server) // multiple server names? else add bool management
{
	std::string arg = next_word_skip();
	if (!arg.length())
		no_arg_error("server_name");
	server->set_name(arg);
	check_semicolon("server_name");
}

void
BuilderCore::parse_server_root(Server *server)
{
	if (_b_server_root)
		duplicate_error("root");
	_b_server_root = true;
	std::string arg = next_word_skip();
	if (!arg.length())
		no_arg_error("root");
	server->set_root(arg);
	check_semicolon("root");
}

void
BuilderCore::parse_server_path_error_page(Server *server)
{
	if (_b_server_path_error_page)
		duplicate_error("path_error_page");
	_b_server_path_error_page = true;
	std::string arg = next_word_skip();
	if (!arg.length())
		no_arg_error("path_error_page");
	server->set_path_error_page(arg);
	check_semicolon("path_error_page");
}

void
BuilderCore::parse_server_auto_index(Server *server)
{
	if (_b_server_auto_index)
		duplicate_error("autoindex");
	_b_server_auto_index = true;
	std::string arg = next_word_skip();
	if (!arg.compare("on"))
		server->set_auto_index(true);
	else if (!arg.compare("off"))
		server->set_auto_index(false);
	else if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("autoindex");
	else if (arg != "")
	{
		std::cerr << "Parsing Error : invalid value \"" << arg << "\" in \"autoindex\" directive, it must be \"on\" or \"off\" on line " << line_count() << std::endl;
		throw (ParsingError());
	}
	check_semicolon("autoindex");
}

void
BuilderCore::parse_server_client_max_body_size(Server *server)
{
	std::locale		loc;
	int				ret;
	size_t			client_max_body_size;

	if (_b_server_client_max_body_size)
		duplicate_error("client_max_body_size");
	_b_server_client_max_body_size = true;
	skip_whitespaces();
	if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("client_max_body_size");
	ret = stoi_skip();
	if (ret < 0)
	{
		std::cerr << "Parsing Error : \"client_max_body_size\" directive invalid value on line " << line_count() << std::endl;
		throw (ParsingError());
	}
	client_max_body_size = ret;
	if (_line[_idx] == 'k' || _line[_idx] == 'K')
		client_max_body_size = client_max_body_size << 10;
	else if (_line[_idx] == 'm' || _line[_idx] == 'M')
		client_max_body_size = client_max_body_size << 20;
	else if (_line[_idx] == 'g' || _line[_idx] == 'G')
		client_max_body_size= client_max_body_size << 30;
	else if (_line.find_first_of(";}# \n\r\t\v\f", _idx) == _idx)
		_idx--;
	else if (!_line[_idx])
		unexpected_eof_error("\";\" or \"}\"");
	else
	{
		std::cerr << "Parsing Error : \"client_max_body_size\" directive invalid value on line " << line_count() << std::endl;
		throw (ParsingError());
	}
	_idx++;
	server->set_client_max_body_size(client_max_body_size);
	check_semicolon("client_max_body_size");
}

void
BuilderCore::parse_server_CGI_param(CGI *cgi)
{
	skip_whitespaces();
	std::string key = next_word_skip();
	std::string value = next_word_skip();
	if (!key.length() || !value.length())
		no_arg_error("cgi_param");
	cgi->add_CGI_param(key, value);
	check_semicolon("cgi_param");
}

void
BuilderCore::parse_server_CGI_exec_name(CGI *cgi)
{
	if (_b_server_CGI_exec_name)
		duplicate_error("exec_name");
	_b_server_CGI_exec_name = true;
	std::string arg = next_word_skip();
	if (!arg.length())
		no_arg_error("exec_name");
	cgi->set_exec_name(arg);
	check_semicolon("exec_name");
}

void
BuilderCore::parse_server_extension(Server *server)
{
	std::string	directive;
	std::string extension_type = next_word_skip();
	skip_whitespaces();
	CGI cgi;
	_b_server_CGI_exec_name = false;
	if (_line [_idx++] != '{')
		no_opening_bracket_error("extension");
	skip_whitespaces();
	while (_line[_idx] && _line[_idx] != '}')
	{
		directive = next_word_skip();
		if (!directive.compare("cgi_param"))
			parse_server_CGI_param(&cgi);
		else if (!directive.compare("exec_name"))
			parse_server_CGI_exec_name(&cgi);
		else if (_line[_idx] ==  ';')
			unexpected_character_error(';');
		else if (directive != "")
			unknown_directive_error(directive);
	}
	if (_line[_idx] != '}')
		unexpected_eof_error("\"}\"");
	server->add_CGI(extension_type, cgi);
	_idx++;
}

void
BuilderCore::parse_server_return(Server *server)
{
	skip_whitespaces();
	int		cursor = _idx;

	if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("return");
	int key = stoi_skip();
	if (key < 0 || key >= 1000 || (!skip_whitespaces() && _line[_idx] != ';'))
	{
		_idx = cursor;
		std::cerr << "Parsing Error : invalid return code \"" << next_word_skip() << "\" on line " << line_count() << std::endl;
		throw(ParsingError());
	}
	std::string value = next_word_skip();
	server->add_return(key, value);
	check_semicolon("return");
}

void
BuilderCore::parse_server(Core *core)
{
	skip_whitespaces();
	std::string	directive;
	Server server;
	erase_server_bool();
	if (_line [_idx++] != '{')
		no_opening_bracket_error("server");
	skip_whitespaces();
	while (_line[_idx] && _line[_idx] != '}')
	{
		directive = next_word_skip();
		if (!directive.compare("listen"))
			parse_server_listen(&server);
		else if (!directive.compare("server_name"))
			parse_server_name(&server);
		else if (!directive.compare("root"))
			parse_server_root(&server);
		else if (!directive.compare("autoindex"))
			parse_server_auto_index(&server);
		else if (!directive.compare("index"))
			parse_server_index(&server);
		else if (!directive.compare("allow_methods"))
			parse_server_allow_methods(&server, core);
		else if (!directive.compare("client_max_body_size"))
			parse_server_client_max_body_size(&server);
		else if (!directive.compare("path_error_page"))
			parse_server_path_error_page(&server);
		else if (!directive.compare("extension"))
			parse_server_extension(&server);
		else if (!directive.compare("return"))
			parse_server_return(&server);
		else if (_line[_idx] ==  ';')
			unexpected_character_error(';');
		else if (directive != "")
			unknown_directive_error(directive);
	}
	if (_line[_idx] != '}')
		unexpected_eof_error("\"}\"");
	_core->add_server(server);
	_idx ++;
}

void
BuilderCore::parse_worker()
{	
	if (_b_worker)
		duplicate_error("worker");
	_b_worker = true;
	skip_whitespaces();
	int		cursor = _idx;
	if (_line[_idx] == ';' || _line[_idx] == '}')
		no_arg_error("worker");
	int	worker = stoi_skip();
	if (worker < 0 || (!skip_whitespaces() && _line[_idx] != ';'))
	{
		_idx = cursor;
		std::cerr << "Parsing Error : invalid number \"" << next_word_skip() << "\""  << " on line " << line_count() << std::endl;
		throw (ParsingError());
	}
	skip_whitespaces();
	if (_line[_idx] == ';')
	{
		_core->set_worker(worker);
		_idx++;
		return ;
	}
	else
		not_terminated_by_semicolon_error("worker");
}

void
BuilderCore::print_debug() const
{
	// std::cerr << &_line[_idx] << std::endl;
	// std::cerr << _idx << std::endl;
	std::cerr << "Worker : " << _core->get_worker() << std::endl;
	_core->print();
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

void
BuilderCore::no_arg_error(std::string directive)
{
	if (_line[_idx] == ';')
		invalid_nb_arguments_error(directive);
	else
		unexpected_character_error('}');
}

void
BuilderCore::parse_mime_type()
{
	Extension * extension = Extension::get_instance();
	_core->set_extension(extension);
}

void
BuilderCore::unexpected_character_error(char character)
{
	std::cerr << "Parsing Error : unexpected \"" << character << "\" on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::unexpected_eof_error(std::string expectation)
{
	std::cerr << "Parsing Error : unexpected end of file, expecting " << expectation << " on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::invalid_nb_arguments_error(std::string directive)
{
	std::cerr << "Parsing Error : invalid number of arguments in " << directive << " directive" << " on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::unknown_directive_error(std::string directive)
{
	std::cerr << "Parsing Error : unknown directive \"" << directive << "\" on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::not_terminated_by_semicolon_error(std::string directive)
{
	std::cerr << "Parsing Error : directive " << directive << " is not terminated by \";\""  << " on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::no_opening_bracket_error(std::string directive)
{
	std::cerr << "Parsing Error : directive " << directive << " has no opening \"{\""  << " on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::host_not_found_error(std::string argument)
{
	std::cerr << "Parsing Error : host not found in \"" << argument << "\" of the \"listen\" directive"  << " on line " << line_count() << std::endl;
	throw(ParsingError());
}

void
BuilderCore::duplicate_error(std::string directive)
{
	std::cerr << "Parsing Error : \"" << directive << "\" directive is duplicate"  << " on line " << line_count() << std::endl;
	throw(ParsingError());
}
