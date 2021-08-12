/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore_errors.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 17:02:01 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/10 17:02:04 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BuilderCore.hpp"

void
BuilderCore::no_arg_error(std::string directive)
{
	if (_line[_idx] == ';')
		invalid_nb_arguments_error(directive);
	else
		unexpected_character_error('}');
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
