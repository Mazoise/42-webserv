/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:13:51 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/29 15:05:50 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core.hpp"
#include <cstring>
#include <fstream>
#include "includes.hpp"

Core::Core(void) :
	_worker(3),
	//_maxfd(-1),
	_nbActive(0)
{
	_SIZE_SOCK_ADDR = sizeof(struct sockaddr_in);
	_methods
		.add_method(new MethodGet())
		.add_method(new MethodDelete())
		;
}

Core::Core(Core const & src)
{
	*this = src;
}

Core::~Core(void)
{

}

Core &	Core::operator=(Core const & rhs)
{
	if (&rhs != this)
		this->_worker = rhs._worker;
	return *this;
}

void	Core::start(){
	int fd;
	std::vector<int> activeSocket;

	// this has to be something we can keep and update
	_fds = new struct pollfd[10];

	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i].start(_worker);
		activeSocket = _servers[i].getActiveSocket();
		_serverSockets.insert(
			_serverSockets.begin(),
			activeSocket.begin(),
			activeSocket.end()
		);
	}
	while (true)
	{

		// value given to poll (nbFds)
		_nbFds = 0;
		for (size_t i = 0; i < _serverSockets.size(); i++)
		{
			fd = _serverSockets[i];
			_fds[_nbFds].fd = fd;
			_fds[_nbFds].events = POLLIN;
			_fds[_nbFds].revents = 0;
			_nbFds++;
		}
		for (size_t i = 0; i < _client.size(); i++)
		{
			fd = _client[i].get_socket();
			_fds[_nbFds].fd = fd;
			_fds[_nbFds].events = POLLOUT;
			_fds[_nbFds].revents = 0;
			_nbFds++;
		}
		_nbActive = poll(_fds, _nbFds, 60000);
		_acceptConnection();
		_handle_request_and_detect_close_connection();
		_detectResetServerPollFD();

		// detect and set serversocket to POLLIN or not
		_detectResetServerPollFD();
	}
}

void	Core::addServer(Server & server)
{
	_servers.push_back(server);
}

Server	&	Core::addServer()
{
	_servers.push_back(Server());
	return(_servers.back());
}

Server	&	Core::getServer(int index)
{
	return (_servers.at(index));
}

void	Core::print()
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i].print();
	}
	if (_servers.size() == 0)
		std::cout << "no Server found \n";
}

void	Core::_acceptConnection()
{
	int new_socket = -1;
	int one = 1;

	for (size_t i = 0; i < _serverSockets.size(); i++)
	{
		int fd = _serverSockets[i];

		if (_fds[i].revents & POLLIN)
		{
			_client.push_back(ClientSocket());
			ClientSocket & cs = _client.back();
			if ((new_socket = accept(fd, (struct sockaddr *)&cs.get_address() , reinterpret_cast<socklen_t*>(&_SIZE_SOCK_ADDR))) < 0)
			{
				perror("accept failed");
				exit(EXIT_FAILURE);
			}
			setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
			std::cout << "New connection, socket fd is " << new_socket << ", socket server :" << fd << std::endl;
			cs.set_socket(new_socket);
			std::cout << "Adding to list of sockets as " << _client.size() << std::endl;
		}
	}
}

void	Core::_handle_request_and_detect_close_connection()
{
	for (client_vector::iterator it = _client.begin(); it != _client.end(); it++)
	{
//		int valread;
//		char buffer[1025];
		BuilderRequest	br(_methods);
		Request * request = 0;
		Response response(200);

		//Check if it was for closing , and also read the 
		//incoming message 
		try{
			br.parse_request(*it);
			request = br.get_request();
			br.reset();
			std::cout << "path : " << request->get_path() << std::endl;
			std::cout << "request : " << request->get_method()->get_name() << std::endl;
			if (request->get_path() == "/")
				request->set_path("/index.html");
			request->set_path("./webserviette_root" + request->get_path());
			request->set_version("HTTP/1.1");

			request->action(response);
		}
		catch (std::exception &e)
		{
			response.setCode(404).clearHeader();
		}
		// std::cout << "parse_request: " <<  parse_ret << std::endl;

		// std::cout << "method: " << request.get_method() << std::endl;

		//if (parse_ret == OK)
		//{
			// std::string ROOT = "./webserviette_root";
			// std::string filename = ROOT + request.get_path();

			delete request;
			ClientSocket & cs = _client.back();
			int clientSocket = cs.get_socket();

			std::cout << "clientSocket: " << clientSocket << std::endl;
			response.sendResponse(clientSocket);

		close( it->get_socket() );  
		_client.erase(it);  
		break;
		//}

//		if ((valread = recv( fd , buffer, 1024, MSG_DONTWAIT)) == 0)  
//		//if ((valread = recv( fd , buffer, 1024, 0)) == 0)  
//		{
//			std::cout << "Host disconnected" << std::endl;  
//
//			close( fd );  
//			_client.erase(it);  
//			break;
//		}
//
//		else if(valread > 0)
//		{
//
//			// debug
//			buffer[valread] = '\0';
//			std::cout << buffer << std::endl;
//
//			// parse_request ?
//			std::stringstream ss;
//			ss << buffer;
//			parse_request(ss, &request);
//
//			// get requested file path
//			std::string ROOT = "./webserviette_root";
//			std::string filename = ROOT + request.get_path();
//
//			std::cout << filename << std::endl;
//			if (filename == ROOT + "/")
//				filename += "index.html";
//
//			// create and send response
//			Response response(200);
//			try
//			{
//				response.setBody(filename);
//			}
//			catch (std::exception & e)
//			{
//				std::cout << e.what() << std::endl;
//
//				filename = ROOT + "/404.html";
//				response.set404(filename);
//			}
//
//			// need client socket
//			ClientSocket & cs = _client.back();
//			int clientSocket = cs.get_socket();
//
//			std::cout << "clientSocket: " << clientSocket << std::endl;
//			response.sendResponse(clientSocket);
//
//
//
//
//			// message for debug, to remove later
//			std::cout << "Server still connected" << std::endl << std::endl;
//
//			//close( fd );  
//			//_client.erase(it);  
//			break;
//		}
	}
}

void	Core::_detectResetServerPollFD()
{
	if (!_client.size())
		for (size_t i = 0; i < _serverSockets.size(); i++)
			if (_fds[i].revents & POLLIN)
				_fds[i].revents = 0;
}
