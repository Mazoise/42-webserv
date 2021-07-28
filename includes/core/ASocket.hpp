/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:44:07 by cdai              #+#    #+#             */
/*   Updated: 2021/07/23 16:41:57 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET
#define ASOCKET

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>
# include "ReaderSocket.hpp"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif

class ASocket
{
	protected:
		int						_socket;
		sockaddr_in				_address;
		int						_id;
		ReaderSocket			_reader;


		void					set_reader(ReaderSocket &);
	public:
		
		ASocket(void);
		ASocket(ASocket const &);
		virtual ~ASocket(void);
		ASocket const &	operator=(ASocket const &);

		int						get_socket(void) const;
		void					set_socket(int socket);
		const sockaddr_in &		get_address(void) const;
		void					set_address(const sockaddr_in & Address);
		int						get_id() const;
		void					set_id(int id);
		ReaderSocket &			get_reader(void);
};

#endif