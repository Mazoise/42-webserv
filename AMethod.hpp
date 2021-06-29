#ifndef AMETHOD
# define AMETHOD

# include <string>
# include "Response.hpp"
# include <cstdio>
# include <exception>

class Request;

class AMethod
{
protected:
	AMethod(void);
	AMethod &	operator=(AMethod const &rhs);
	AMethod(AMethod const & src);
	
	std::string	_name;

public:

	AMethod(std::string name);
	virtual ~AMethod(void);

	const std::string & get_name() const;

	virtual void	action(const Request &, Response &) = 0;

};

# include "Request.hpp"

#endif
