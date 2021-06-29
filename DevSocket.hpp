// cdai - temp file
#ifndef DEVSOCKET
#define DEVSOCKET

#include "ASocket.hpp"

class DevSocket : public ASocket
{
	public:
		DevSocket(void);
		DevSocket(int socket);
		virtual ~DevSocket(void);

	private:
} ;

#endif
