#ifndef STRATEGYERROR
# define STRATEGYERROR

# include "IResponseStrategy.hpp"

class StrategyError : public IResponseStrategy
{
	int	_code;
public:

	StrategyError(int code = 500);
	StrategyError(StrategyError const & src);
	virtual ~StrategyError(void);
	StrategyError &	operator=(StrategyError const &rhs);

private:
	virtual Response * create(Server &, Request &);
};

#endif
