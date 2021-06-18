/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:13:48 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/18 15:56:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE
# define CORE

# include "Server.hpp"
# include <vector>
# include "ClientSocket.hpp"
# include <poll.h>

class Core
{
public:
	typedef std::vector<ClientSocket> client_vector;

	Core(void);
	Core(Core const & src);
	virtual ~Core(void);
	Core &	operator=(Core const &rhs);

	void	start();
	void	addServer(Server & server);
	void	addServer();
	Server	&	getServer(int index = 0);

	void	print();

private:

	void				_acceptConnection(struct pollfd fds[]);
	void				_detectCloseConnection(struct pollfd fds[]);

	std::vector<Server>	_servers;
	int					_worker;
	//fd_set				_readfds;
	int					_maxfd;
	int					_nbActive;
	std::vector<int>	_serverSockets;
	client_vector		_client;
	int					_SIZE_SOCK_ADDR = sizeof(struct sockaddr_in);
};

#endif
