#ifndef METHODDELETE
# define METHODDELETE

#include "AMethod.hpp"

class MethodDelete : public AMethod
{
	MethodDelete &	operator=(MethodDelete const &rhs);
	MethodDelete(MethodDelete const & src);
public:

	MethodDelete(void);
	virtual ~MethodDelete(void);

	virtual void action(const Request &, Response &);

private:

};

#endif
