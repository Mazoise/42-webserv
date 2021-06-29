#include "BuilderRequest.hpp"

BuilderRequest::BuilderRequest(MethodLibrary & methods) : _request(new Request()), _methods(methods) {}

BuilderRequest::~BuilderRequest(void)
{
	delete _request;
}

int		BuilderRequest::add_method(std::string line)
{
	int ret = line.find(' ');
	std::string name = line.substr(0, ret);

	_request->set_method(_methods.get_method(name));
	if (!_request->get_method())
		throw MethodNotImplemented();
	return (ret + 1);
}

int		BuilderRequest::add_path(std::string line)
{
	size_t		len = line.find(' ');
	if (len == std::string::npos)
		throw BadResquest();
	_request->set_path(line.substr(0, len));
	// check characters
	return(len + 1);
}

int		BuilderRequest::add_version(std::string line)
{
	if (line.compare(0, 8, VERSION))
		throw BadHttpVesion();
	_request->set_version(VERSION);
	return (8);
}

void	BuilderRequest::first_line(std::string line)
{
	int	j = add_method(line);
	j += add_path(&line[j]);
	j += add_version(&line[j]);
	if (line[j] != '\r')
		throw BadResquest();
}

bool	BuilderRequest::parse_headers(std::string line)
{
	size_t		len = line.find(": ");

	if (line.length() == 1 && line[0] == '\r')
		return false;
	if (line[line.length() - 1] != '\r' || len == std::string::npos || line[len - 1] == ' ')
		throw BadResquest();
	_request->add_header(std::pair<std::string, std::string>(line.substr(0, len), line.substr(len + 2, line.length() - len - 3)));
	return (true);
}

void	BuilderRequest::parse_request(ASocket & socket)
{
	std::string		line;
	bool			is_first_line = true;
	int				gnl_ret = 1;

	while( gnl_ret && (gnl_ret = socket.get_next_line(line)))
	{
//		std::cout << "gnl_ret: " << gnl_ret << std::endl;
//		std::cout << "line: " << line << std::endl;

		line += "\r"; // Maybe, we can remote this line ? (from cdai)
		//check printable characters
		if (is_first_line)
		{
			first_line(line);
			is_first_line = false;
		}
		else{
			if (!parse_headers(line))
				gnl_ret = 0;
		}
	}
}

Request * BuilderRequest::get_request() const
{
	return _request;
}

void	BuilderRequest::reset()
{
	_request = new Request();
}