/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CsvReader.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:37:16 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 21:12:17 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CSV_READER_HPP
# define CSV_READER_HPP

# include "Reader.hpp"
# include "ExitException.hpp"
# include <vector>
# include <map>
# include <sstream>

# include <iostream>

class CsvReader : public Reader
{
	public:

		typedef	std::vector<std::string>	vector;
		typedef std::map< std::string, vector> map;

	private:

		vector				_categories;
		std::string			_key_name;
		map					_content;
		
	public:

		CsvReader(void);
		CsvReader(const std::string &);
		CsvReader(CsvReader const & src);
		virtual ~CsvReader(void);
		CsvReader &	operator=(CsvReader const &rhs);

		bool		has_category(const std::string &) const;
		CsvReader &	set_key_name(const std::string &);
		CsvReader &	parse_categeries();
		CsvReader &	parse_content();

		std::string operator[](const std::string &) const;
};

#endif
