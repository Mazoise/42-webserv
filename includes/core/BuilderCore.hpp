/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BuilderCore.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/23 17:04:45 by mchardin          #+#    #+#             */
/*   Updated: 2021/08/03 23:41:57 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_CORE_HPP
# define BUILDER_CORE_HPP

#include "Core.hpp"
#include <locale>

class Core;

class BuilderCore
{
	private:

		size_t			_idx;
		std::string		_line;
		Core			*_core;
		std::string		_error_msg;

		bool			_b_worker;
		bool			_b_server_name; //?
		bool			_b_server_root;
		bool			_b_server_path_error_page;
		bool			_b_server_auto_index;
		bool			_b_server_client_max_body_size;
		bool			_b_server_CGI_exec_name;

		std::string		next_word_skip();
		int				skip_whitespaces();
		void			skip_comments();
		int				line_count();
		void			erase_server_bool();
		std::string		check_return_ip(int first_nb, int cursor);
		void			parse_server_listen(Server *server);
		void			parse_server_name(Server *server);
		void			parse_server_root(Server *server);
		void			parse_server_path_error_page(Server *server);
		void			parse_server_auto_index(Server *server);
		void			parse_server_index(Server *server);
		void			parse_server_allow_methods(Server *server, Core *core);
		void			parse_server_client_max_body_size(Server *server);
		void			parse_server_CGI_param(CGI *cgi);
		void			parse_server_CGI_exec_name(CGI *cgi);
		void			parse_server_extension(Server *server);
		void			parse_server_return(Server *server);
		void			parse_server(Core *core);
		void			parse_worker();
		int				stoi_skip();
		void			check_semicolon(std::string directive);
		void			no_arg_error(std::string directive);
		void			unexpected_character_error(char character);
		void			unexpected_eof_error(std::string expectation);
		void			invalid_nb_arguments_error(std::string directive);
		void			unknown_directive_error(std::string directive);
		void			not_terminated_by_semicolon_error(std::string directive);
		void			no_opening_bracket_error(std::string directive);
		void			host_not_found_error(std::string argument);
		void			duplicate_error(std::string directive);

	public:

		BuilderCore(std::istream &fd, Core *core);
		~BuilderCore();
		void			print_debug() const; //tmp
		Core *			get_builded_core() const;
		void			parse_mime_type();
	
		class ParsingError : public std::exception
		{
				virtual const char*	what() const throw(){
					return ("Parsing Error");
			}
		};
};

#endif