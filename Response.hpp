/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:02:41 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/25 16:12:33 by cdai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE
# define RESPONSE

# include <string>
# include <map>
# include <utility>
# include <sstream>

# include <unistd.h>
# include <fstream>
# include "Reader.hpp"

class Response
{
public:
	typedef std::map<std::string, std::string>	header_map;

	Response(int code = 200);
	Response(Response const & src);
	virtual ~Response(void);
	Response &	operator=(Response const &rhs);

	Response &	addHeader(std::string name, std::string content);
	void		clearHeader();
	std::string	getResponse();
	void		sendResponse(int fd);
	std::string	getMessage(int code);
	Response &	setCode(int code);
	Response &	setBody(const std::string & filename);
	Response &	set404(std::string & filename);

private:

	std::string		_1xx__response(int code);
	std::string		_2xx__response(int code);
	std::string		_3xx__response(int code);
	std::string		_4xx__response(int code);
	std::string		_5xx__response(int code);

	std::string		_version;
	int				_code;
	header_map		_headers;
	std::string		_body;

};

#endif
