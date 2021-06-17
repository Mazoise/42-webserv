/* source: https://www.binarytides.com/socket-programming-c-linux-tutorial/ */
#include "cdai_test.h"

int main()
{
	int socket_desc , new_socket , c;
	int socket_desc2 ;//, new_socket2 , c2;
	struct sockaddr_in server , client;
	struct sockaddr_in server2 ;//, client2;



	//Create socket
	//Set option on socket_fd (like REUSEADDR to rebind (and so, re-test quickly))
	socket_desc = get_new_socket();
	socket_desc2 = get_new_socket();
	printf("socket_desc: %d\n", socket_desc);
	printf("socket_desc2: %d\n", socket_desc2);



	//Prepare the sockaddr_in structure
	init_server(&server, 8888);
	init_server(&server2, 8080);



	//Bind socket to ip addr
	server_bind(socket_desc, &server);
	server_bind(socket_desc2, &server2);


	//Listen
	listen(socket_desc , 3);
	listen(socket_desc2 , 3);


	struct pollfd fds[10];

	fds[0].fd = socket_desc;
	fds[0].events = POLLIN;
	fds[1].fd = socket_desc2;
	fds[1].events = POLLIN;

	while (1)
{
	printf("POLLIN: %d\n", POLLIN);
	int poll_ret = poll(fds, 2, 50000);
	printf("poll_ret: %d\n", poll_ret);

	for(int j = 0; j < 2; j++)
	{
		printf("fds[%d].events: %d\n", j, fds[j].events);
		printf("fds[%d].revents: %d\n", j, fds[j].revents);

	}
for (int i = 0; i < 2; i++)
if (fds[i].revents & POLLIN)
{
	for(int j = 0; j < 2; j++)
		printf("fds[%d].events: %d\n", j, fds[j].events);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	struct sockaddr_in client_test;
	(void)client;

//	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	while( (new_socket = accept(fds[i].fd, (struct sockaddr *)&client_test, (socklen_t*)&c)) )
	{
		puts("---- ---- ---- Connection accepted ---- ---- ----");

		int read_size, length; //recv return value, i think it's like read
		//char client_message[2000], answer[10000], *header, *response, *to_free;
		char client_message[2000], *response;

		// recv is a kind of read
		if( (read_size = recv(new_socket , client_message , 2000 , 0)) > 0 )
		{
			printf("---- read_size: %d ----\n", read_size);

			client_message[read_size] = 0;
			// print in stdout the request
			write(1, client_message , strlen(client_message));
		}


		// like ctrl + d, i think, or EOF
		if(read_size == 0)
		{
			puts("Client disconnected");
			fflush(stdout);
		} // handle recv error
		else if(read_size == -1)
			perror("recv failed");
		else
		{
			response = build_response("index.html");

			length = strlen(response);
			write(1, response, length);

			// do the same than write
			send(new_socket, response, length, 0);

			free(response);
			// if the socket is not closed, the broswer continue working
			close(new_socket);
		}

		// useless. Maybe we can handle signal ?
		if (!strcmp(client_message, "CLOSE\r\n"))
			break;

	fds[i].revents = 0;
	break;
	}

	// handle accept() fail
	if (new_socket < 0)
	{
		perror("accept failed");
		return 1;
	}

} //if
} //while
	return 0;
}

