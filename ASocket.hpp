/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdai <cdai@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 16:44:07 by cdai              #+#    #+#             */
/*   Updated: 2021/06/29 11:31:46 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET
#define ASOCKET

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif

class ASocket
{
public:
	
	virtual ~ASocket(void);

	int				get_socket(void) const;
	void			set_socket(int socket);
	sockaddr_in	&	get_address(void);
	void			set_address(sockaddr_in & Address);
	int				get_next_line(std::string & str);

protected:
	int			_socket;
	sockaddr_in	_address;

};

#endif
