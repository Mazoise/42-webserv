/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AReaderFileDescriptor.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/23 11:11:35 by cdai              #+#    #+#             */
/*   Updated: 2021/11/19 21:12:03 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AREADER_FILE_DESCRIPTOR_HPP
# define AREADER_FILE_DESCRIPTOR_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <exception>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1025
# endif

class AReaderFileDescriptor
{
	public:
	class ReadError : public std::exception
	{
		virtual const char*	what() const throw(){
			return "Read error";
		}
	};

	class WriteError : public std::exception
	{
		virtual const char*	what() const throw(){
			return "Write error";
		}
	};
	class EndOfFile : public std::exception
	{
		virtual const char*	what() const throw(){
			return "End of file";
		}
	};
	
	protected:

		int			_fd;
		size_t		_size;
		char		_buffer[BUFFER_SIZE];
		std::string	_chunck;
		size_t		_account_body;
		size_t		_limit_body;

		AReaderFileDescriptor(void);

	public:
		class OutOfBound : public std::exception
		{
			virtual const char* what() const throw(){
				return "Out of Bound";
			}
		};
		class OutOfBuffer : public std::exception
		{
			virtual const char* what() const throw(){
				return "Out of Buffer";
			}
		};
		AReaderFileDescriptor(int fd);
		AReaderFileDescriptor(AReaderFileDescriptor const &);
		virtual ~AReaderFileDescriptor(void);
		AReaderFileDescriptor const & operator=(AReaderFileDescriptor const &);

		int			get_next_line(std::string &);
		void		read_body(std::string &, int);
		void		read_until_end(std::string &);
		int			get_fd(void);
		void		set_fd(int);
		std::string	get_buffer(void) const;
		std::string	& get_chunck();
		size_t		get_account_body() const;
		void		set_account_body(size_t);
		size_t		get_size() const;
		void		set_limit(size_t);

		void		_reset_buffer(void);
		virtual int	_read(void *, int n_read = BUFFER_SIZE - 1) = 0;
		int			next_read(size_t size = BUFFER_SIZE - 1);
		int			fill_buffer();
		int			write_body(int fd);
		bool		body_full() const;

		bool		has_line() const;
		bool		has_all_headers() const;
		void		concatenation();
		void		reset_chunck();
		void		cut_header();
		void		move_buffer_until(size_t pivot = BUFFER_SIZE - 1);

};

#endif
