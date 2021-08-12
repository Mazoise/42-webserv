/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore_cgi.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 17:01:45 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/10 17:01:46 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BuilderCore.hpp"

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
