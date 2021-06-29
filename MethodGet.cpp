#include "MethodGet.hpp"

MethodGet::MethodGet(void) : AMethod("GET") {}


MethodGet::~MethodGet(void)
{
	
}

void	MethodGet::action(const Request & request, Response & response)
{	try
	{
		response.setCode(200).setBody(request.get_path().c_str());
	}
	catch(const std::exception& e)
	{
		response.setCode(404).clearHeader();
	}
}
