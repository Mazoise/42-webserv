/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodDelete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syndraum <syndraum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 15:50:12 by mchardin          #+#    #+#             */
/*   Updated: 2021/11/19 20:11:56 by syndraum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MethodDelete.hpp"

MethodDelete::MethodDelete(void) :
AMethod("DELETE")
{}

MethodDelete::~MethodDelete() {}

void
MethodDelete::action(const Request & request, Response & response, Server & server, AReaderFileDescriptor & reader)
{
	if (!reader.body_full())
	{
		reader._reset_buffer();
		return ;
	}
	if (0 != (std::remove(request.get_path().c_str())))
	{
		response.set_error(404, server.get_path_error_page());
	}
	else
	{
		response.set_code(204);
	}
	finished();
}

AMethod *
MethodDelete::clone() const
{
	AMethod * clone = new MethodDelete();
	*clone = *this;
	return (clone);
}