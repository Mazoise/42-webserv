/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirection.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 14:36:35 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 21:10:49 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION
# define REDIRECTION

# include <string>

class Redirection
{
	private:
	
		int			_code;
		std::string	_ressource;

		Redirection(void);

	public:

		Redirection(int, const std::string &);
		Redirection(Redirection const & src);
		virtual ~Redirection(void);
		Redirection &	operator=(Redirection const &rhs);

		bool				is_handler() const;
		int					get_code() const;
		const std::string &	get_ressource() const;

};

#endif
