#include "AMethod.hpp"

AMethod::AMethod(void) : _name() {}

AMethod::AMethod(std::string name) : _name(name) {}

AMethod::~AMethod(void)
{
	
}

const std::string & AMethod::get_name() const
{
	return (_name);
}
