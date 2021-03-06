/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:14:00 by syndraum          #+#    #+#             */
/*   Updated: 2021/11/19 20:52:11 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <cerrno>
# include <cstring>
# include "ASocket.hpp"
# include <exception>

class ServerSocket : public ASocket
{
	private:

		int					_port;
		std::string			_ip;
		bool				_active;
		
	public:
	
		ServerSocket(int port = 80);
		ServerSocket(int port, std::string ip);
		ServerSocket(ServerSocket const & src);
		virtual ~ServerSocket(void);
		ServerSocket &	operator=(ServerSocket const &rhs);

		void				setup_socket();
		void				bind_socket();
		void				listen_socket(int worker_connection);

		ServerSocket *		set_port(int port);
		int					get_port() const;
		const std::string &	get_ip() const;
		ServerSocket *		set_active(bool);
		bool				get_active() const;

		class SocketError : public std::exception
		{
			virtual const char*	what() const throw(){
				return "Socket error";
			}
		};
		class BindError : public std::exception
		{
			virtual const char*	what() const throw(){
				return "Bind error";
			}
		};
		class ListenError : public std::exception
		{
			virtual const char*	what() const throw(){
				return "Listen error";
			}
		};
};

#endif
