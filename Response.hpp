/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 12:02:41 by syndraum          #+#    #+#             */
/*   Updated: 2021/06/18 15:41:34 by syndraum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE
# define RESPONSE

# include <string>
# include <map>
# include <utility>
# include <sstream>

#include <unistd.h>

class Response
{
public:
	typedef std::map<std::string, std::string>	header_map;

	Response(int code = 200);
	Response(Response const & src);
	virtual ~Response(void);
	Response &	operator=(Response const &rhs);

	Response &	addHeader(std::string name, std::string content);
	std::string	getResponse();
	void		sendResponse(int fd);
	std::string	getMessage(int code);

private:
	std::string		_version;
	int				_code;
	header_map		_headers;
	std::string		_body;

};

#endif
