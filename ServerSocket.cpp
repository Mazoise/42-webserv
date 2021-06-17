#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int port) : _port(port), _socket(-1)
{

}

ServerSocket::ServerSocket(ServerSocket const & src)
{
	*this = src;
}

ServerSocket::~ServerSocket(void)
{
	
}

ServerSocket &	ServerSocket::operator=(ServerSocket const & rhs)
{
	if (&rhs != this){
		this->_port = rhs._port;
		this->_socket = rhs._socket;
		this->_address = rhs._address;
	}
	return *this;
}

void			ServerSocket::setupSocket()
{
	_socket = socket(AF_INET , SOCK_STREAM , 0);
	if (_socket == -1)
		throw "Socket error";
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons( _port );
}

void			ServerSocket::bindSocket()
{
	if ((bind(_socket, reinterpret_cast<sockaddr*>(&_address), sizeof(_address)) < 0)){
		std::cerr << strerror(errno) << std::endl;
		throw std::exception();
	}
	std::cout << "bind done. Listen on port " << _port <<std::endl;
}

void			ServerSocket::listenSocket(int worker_connection)
{
	if ((listen(_socket, worker_connection)) < 0)
		throw "listen failed";
	std::cout << "Waiting for incoming connection..." << std::endl;
}

ServerSocket *	ServerSocket::setPort(int port)
{
	_port = port;
	return this;
}

int				ServerSocket::getPort()
{
	return (_port);
}

int				ServerSocket::getSocket()
{
	return (_socket);
}

sockaddr_in &		ServerSocket::getServer()
{
	return (_address);
}

void	ServerSocket::print()
{
	std::cout << " adress : " << this << "\n";
	std::cout << " port : " << _port << "\n";
	std::cout << " fd : " << _socket << "\n";
}