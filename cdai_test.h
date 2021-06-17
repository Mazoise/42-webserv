#ifndef CDAI_TEST_H
#define CDAI_TEST_H

#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

#include<pthread.h> //for threading , link with lpthread
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>
#include <poll.h>

int get_new_socket(void); // return a fd
void	init_server(struct sockaddr_in *server, int port); // fullfill a (struct sockaddr_in)
int	server_bind(int fd, struct sockaddr_in * server); // use bind
char *build_response(char *file_name);
char *strjoin(char *str1, char *str2);


#endif
