/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:43:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:03:17 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketException.hpp"

tlucanti::SocketException::SocketException(const std::string &message,
	int error)
{
	_message = message;
	_message = "[r][FAIL][w] socket:[] " + _message + ": [y]" +
		ft_strerror(error) + "[]";
}

const std::string &
tlucanti::SocketException::what()
{
	return _message; 
}

const char *
tlucanti::SocketException::ft_strerror(int errnum)
{
	switch (errnum)
	{
		case 0: return "";
		default: return strerror(errnum);
	}
}
