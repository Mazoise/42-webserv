#include "cdai_test.h"

void init_server(struct sockaddr_in *server, int port)
{
	server->sin_family = AF_INET;
	server->sin_addr.s_addr = INADDR_ANY;
	server->sin_port = htons( port );
}
