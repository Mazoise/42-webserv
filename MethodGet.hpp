#ifndef METHODGET
# define METHODGET

# include "AMethod.hpp"

class MethodGet : public AMethod
{
	MethodGet &	operator=(MethodGet const &rhs);
	MethodGet(MethodGet const & src);
public:
	MethodGet(void);
	virtual ~MethodGet(void);

	virtual void action(const Request &, Response &);

private:

};

#endif
