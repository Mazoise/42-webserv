/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:37:25 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 21:13:03 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD
# define UPLOAD

# include <string>
# include <fstream>
# include "Request.hpp"
# include "AReaderFileDescriptor.hpp"
# include "Server.hpp"
# include "Message.hpp"
# include "StringPP.hpp"

class Upload
{
	public:
	
		enum e_state{
			FIND_BOUNDARY,
			HEADER,
			WRITE,
			END,
		};

	private:

		AReaderFileDescriptor *	_reader;
		Upload::e_state			_state;
		std::string				_boundary;
		std::string				_buffer;
		size_t					_position;
		std::string				_filename;
		std::fstream			_file;
		Message					_message;

	public:

		class BadMessage : public std::exception
		{
			virtual const char* what() const throw(){
				return "Bad message";
			}
		};
		class InvalidReader : public std::exception
		{
			virtual const char* what() const throw(){
				return "Invalid Reader";
			}
		};
		Upload(AReaderFileDescriptor * reader = 0);
		Upload(Upload const & src);
		virtual ~Upload(void);
		Upload &	operator=(Upload const &rhs);

		Upload *	set_reader(AReaderFileDescriptor &);

		void	upload(Server &, const Request &);
		void	set_boundary(const Request &);
		void	set_filename(const Message &);
		e_state	get_state() const;
		void	next_position();
		bool	has_boundary();
		void	find();
		void	header(const Server &);
		void	write();
};

#endif
