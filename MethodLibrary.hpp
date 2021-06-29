#ifndef METHODLIBRARY
# define METHODLIBRARY

# include <vector>
# include "AMethod.hpp"

class MethodLibrary
{
	MethodLibrary(MethodLibrary const & src);
	MethodLibrary &	operator=(MethodLibrary const &rhs);
public:

	MethodLibrary(void);
	virtual ~MethodLibrary(void);

	MethodLibrary &	add_method(AMethod *);
	AMethod	* get_method(const std::string &) const;

private:
	std::vector<AMethod *> _methods;
};

#endif
