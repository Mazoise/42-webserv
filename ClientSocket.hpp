/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:13:56 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/29 11:48:04 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET
# define CLIENTSOCKET

# include <sys/socket.h>
# include <netinet/in.h>
# include "ASocket.hpp"

class ClientSocket : public ASocket
{
public:

	ClientSocket(void);
	ClientSocket(ClientSocket const & src);
	virtual ~ClientSocket(void);
	ClientSocket &	operator=(ClientSocket const &rhs);

private:
};

#endif
