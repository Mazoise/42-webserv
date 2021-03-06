/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Extension.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:37:19 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 21:12:33 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXTENSION_HPP
# define EXTENSION_HPP

# include <string>
# include <exception>
# include "CsvReader.hpp"

class Extension
{
	Extension(Extension const & src);
	Extension &	operator=(Extension const &rhs);

	protected:

		static Extension *	_extension;
		CsvReader			_csv_reader;
		Extension(void);
		
	public:

		virtual ~Extension(void);

		static std::string	get_extension(const std::string & path);
		static std::string	get_mine_type(const std::string & ext, CsvReader & csv);
		static Extension *	get_instance(void);
		static void			delete_instance(void);
		const CsvReader &	get_reader(void) const { return _csv_reader; }
};

#endif
