/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore_server.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 17:01:40 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/10 17:01:41 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BuilderCore.hpp"

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
