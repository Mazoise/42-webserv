#ifndef READER
# define READER

# include <string>
# include <fstream>

class Reader
{
public:

	Reader(void);
	Reader(const std::string &);
	Reader(Reader const & src);
	virtual ~Reader(void);
	Reader &	operator=(Reader const &rhs);

	void	open();
	void	to_string(std::string & );
	void	close();

	int		get_length() const;
private:
	std::string		_path;
	std::ifstream	_ifs;
	int				_length;
};

#endif
