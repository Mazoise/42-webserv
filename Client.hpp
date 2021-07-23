#ifndef CLIENT
# define CLIENT

# include "ClientSocket.hpp"
# include "Server.hpp"
# include "Request.hpp"

class Client
{
	Request			_request;
	ClientSocket	_socket;
	Server &		_server;
	ServerSocket &	_server_socket;

	Client(void);
public:

	Client(Server &, ServerSocket &);
	Client(Client const & src);
	virtual ~Client(void);
	Client &	operator=(Client const &rhs);

	Request &		get_request();
	ClientSocket &	get_socket_stuct();
	int				get_socket();
	Server &		get_server();
private:

};

#endif
