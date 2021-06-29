/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 20:58:40 by mchardin          #+#    #+#             */
/*   Updated: 2021/06/29 14:50:16 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "Core.hpp"
#include "Server.hpp"
#include "MethodGet.hpp"
#include "MethodDelete.hpp"
#include "includes.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// file to delete
//#include "DevSocket.hpp"
int		main()
{
//GNL tester, from cdai
//	int fd = open("./main.cpp", O_RDONLY);
//	std::cout << "fd: " << fd << std::endl;
//	std::string str;
//	DevSocket socket(fd);
//	int gnl_ret = 1;
//
//	while ((gnl_ret = socket.get_next_line(str)))
//	{
//		std::cout << "gnl_ret: " << gnl_ret << std::endl;
//		std::cout << "str: " << str << std::endl;
//	}
//	std::cout << "gnl_ret: " << gnl_ret << std::endl;
//	std::cout << "str: " << str << std::endl;


	Core core = Core();
	core.addServer()
		.addPort(8888)
		.addPort(3333)
		;
	core.start();

//maia's part
// #include "includes.hpp"
// #include "BuilderRequest.hpp"

// std::string	error_msg(int code) // ADD TO RESPONSE
// {
// 	if (code == BAD_REQUEST)
// 		return ("Bad Request");
// 	else if(code == BAD_VERSION)
// 		return ("HTTP Version Not Supported");
// 	else if(code == NOT_IMPLEMENTED)
// 		return("Not Implemented");
// 	return (0);
// }

// int		print_error(int code) // ADD TO RESPONSE
// {
// 	std::cout << code << " " << error_msg(code) << std::endl;
// 	return(code);
// }

// int		main() // run with "cat test_request | ./webserv"
// {
// 	BuilderRequest bd;
// 	Request	*	request = 0;
// 	// int			code;

// 	try
// 	{	
// 		bd.parse_request(std::cin);
// 		request = bd.get_request();
// 		bd.reset();
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	// code = parse_request(std::cin, &request);
// 	// if (code != OK)
// 	// 	return(print_error(code));
// 	if (request)
// 		request->print_debug();
// 	delete request;
}
