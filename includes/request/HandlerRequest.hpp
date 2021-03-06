/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerRequest.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:37:32 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 21:13:29 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_REQUEST_HPP
# define HANDLER_REQUEST_HPP

# include <exception>
# include "Request.hpp"
# include "Server.hpp"
# include "BuilderRequest.hpp"
# include "Client.hpp"
# include "StrategyAccept.hpp"
# include "StrategyError.hpp"
# include "StrategyIndex.hpp"
# include "StrategyCGI.hpp"
# include "StrategyReturn.hpp"
# include <poll.h>

class HandlerRequest
{
	public:
	
		typedef std::vector<Client>		clients;
		typedef std::vector<Server>		servers;
		typedef clients::iterator		clients_iterator;
		
	private:

		Client *			_client;
		BuilderRequest &	_builder;
		size_t				_account;

		HandlerRequest(void);

	public:

		HandlerRequest(BuilderRequest &);
		HandlerRequest(HandlerRequest const & src);
		virtual ~HandlerRequest(void);
		HandlerRequest &	operator=(HandlerRequest const &rhs);

		HandlerRequest &	set_client(Client *);
		Request &			get_request();
		const Request &		get_request() const;
		Server &			get_server();
		ClientSocket &		get_client_socket();
		size_t				get_account() const;

		int					handle(Client &, servers &);
		void				read_header(servers &);
		void				parse();
		void				set_index();
		bool				is_complete() const;
		void				check_host(servers &);
		void				check_body_size(Client const &) const;
		void				check_method_exist(Client const &) const;
		int					clean();

		class BodyTooLong : public std::exception
		{
			virtual const char*	what() const throw(){
				return "Body toot long";
			}
		};
		class MethodNotAllowed : public std::exception
		{
			virtual const char*	what() const throw(){
				return "Method not allowed";
			}
		};
		class RecvError : public std::exception
		{
			virtual const char*	what() const throw(){
				return "Recv error";
			}
		};
};

#endif
