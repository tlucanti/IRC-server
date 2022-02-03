/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:43:54 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:01:37 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_EXCEPTION_HPP
# define SOCKET_EXCEPTION_HPP

#include <stdexcept>
#include <cstring>
#include <string>

namespace tlucanti
{
	class SocketException : public std::exception
	{
	public:
		SocketException(const std::string &message, int error);
		const std::string &what();

	protected:
		const char *ft_strerror(int errnum);
		std::string _message;
	};
}

#endif	// SOCKET_EXCEPTION_HPP
