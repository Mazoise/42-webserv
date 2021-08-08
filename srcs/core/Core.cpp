/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:13:51 by syndraum          #+#    #+#             */
/*   Updated: 2021/08/03 21:12:55 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core.hpp"

Core::Core(void) :
_worker(3)
{
	_SIZE_SOCK_ADDR = sizeof(struct sockaddr_in);
	_methods
		.add_method(new MethodGet())
		.add_method(new MethodDelete())
		.add_method(new MethodPost())
		;
	_br.set_library(&_methods);
}

Core::Core(Core const & src)
{
	*this = src;
}

Core::~Core()
{

}

Core &
Core::operator=(Core const & rhs)
{
	if (&rhs != this)
		this->_worker = rhs._worker;
	return *this;
}

int
Core::init(int argc, char * argv[], char *env[])
{
	std::string path_config_file = Info::path_config_file;

	if (argc > 2)
	{
		std::cerr << "Error: too many arguments" << std::endl;
		return (2);
	}
	if (argc == 2)
		path_config_file = argv[1];
	
	Reader reader(path_config_file);
	try
	{
		reader.open();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: Configuration file: " << path_config_file << " not found" << std::endl;
		return (1);
	}
	try
	{
		BuilderCore builder_core(reader.get_ifs(), this);
		builder_core.parse_mime_type();
		builder_core.print_debug(); // 
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << '\n';
		return (3);
	}
	Info::env = env;
	return (0);
}

void
Core::start()
{
	HandlerRequest		hr(_br);
	std::vector<int>	active_socket;
	try
	{
		for (size_t i = 0; i < _servers.size(); i++)
			_servers[i].start(_worker);
	}
	catch(std::exception & e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return ;
	}
// print();
	_pfdh.init(_servers);
	while (true)
	{
//		poll(&(_pfdh.get_pfd().front()), _pfdh.get_pfd().size(), 60000);
		_pfdh.watch();
		_pfdh.accept_connection(_servers, _client);
//		_accept_connection();

		client_vector::iterator client = hr.handle(_client, _servers);
		if (client != _client.end()){
			remove_client(client);
			// return ;
		}
		// _pfdh.reset_server();
	}
}

void
Core::clean()
{
	Extension::delete_instance();
	_client.clear();
	_servers.clear();
}

void
Core::add_server(Server & server)
{
	_servers.push_back(server);
}

Server &
Core::add_server()
{
	_servers.push_back(Server());
	return(_servers.back());
}

Server	const &
Core::get_server(int index) const
{
	return (_servers.at(index));
}

int	
Core::get_worker(void) const
{
	return (_worker);
}

Core &
Core::set_worker(int worker)
{
	_worker = worker;
	return(*this);
}

void
Core::set_extension(Extension * extension)
{
	_extension = extension;
}

bool
Core::has_host_port(const std::string & ip, int port) const
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		const Server & server = _servers[i];

		for (Server::port_map::const_iterator it = server.get_map_socket().begin();
			it != server.get_map_socket().end(); it++)
		{
			const ServerSocket & server_socket = it->second;

			if (server_socket.get_port() == port && ip == server_socket.get_ip())
				return true;
		}
	}
	return false;
}

void
Core::print() const
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i].print();
	}
	if (_servers.size() == 0)
		std::cout << "no Server found \n";
}

void
Core::remove_client(client_vector::iterator it)
{
	_pfdh.erase();
	close(it->get_socket());
	_client.erase(it);
	std::cout << "Disconnected" << std::endl;
}

AMethod *
Core::get_method(std::string const & name) const
{
	return (_methods.get_method(name));
}
