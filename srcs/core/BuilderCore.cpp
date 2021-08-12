/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 17:04:45 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/12 16:47:27 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BuilderCore.hpp"

BuilderCore::BuilderCore(Core *core) :
_idx(0),
_core(core),
_b_worker(false)
{}

BuilderCore::~BuilderCore() {}

void
BuilderCore::build(std::istream & fd)
{
	std::string	directive;
	std::getline(fd, _line, char(EOF));
	while (_idx < _line.length())
	{
		directive = next_word_skip();
		if (!directive.compare("worker"))
			parse_worker();
		else if (!directive.compare("server"))
			parse_server(_core);
		else if (_line[_idx] ==  ';')
			unexpected_character_error(';');
		else if (_line[_idx] ==  '}')
			unexpected_character_error('}');
		else if (directive != "")
			unknown_directive_error(directive);
	}
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

Core *
BuilderCore::get_builded_core() const
{
	return (_core);
}

void
BuilderCore::parse_mime_type()
{
	Extension * extension = Extension::get_instance();
	_core->set_extension(extension);
}

void
BuilderCore::print_debug() const
{
	// std::cerr << &_line[_idx] << std::endl;
	// std::cerr << _idx << std::endl;
	std::cerr << "Worker : " << _core->get_worker() << std::endl;
	_core->print();
}
