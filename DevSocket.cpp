// cdai - temp file
#include "DevSocket.hpp"

DevSocket::DevSocket(void)
{
	_socket = 0;
}

DevSocket::DevSocket(int socket)
{
	_socket = socket;
}

DevSocket::~DevSocket(void)
{}
