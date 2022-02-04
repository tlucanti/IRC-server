/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:56:47 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:02:57 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

namespace tlucanti
{
	__WUR Socket accept(const Socket &_sock)
	{
		int int_sock = ::accept(_sock.get_sock(), nullptr, nullptr);
		if (int_sock < 0)
			throw SocketException("cannot accept socket", errno);
		return {int_sock};
	}

	__WUR std::string recv(const Socket &_sock)
	{
		std::string message;
		char buff[Socket::READ_SIZE];

		ssize_t rbytes;

		while (true)
		{
			rbytes = ::recv(_sock.get_sock(), buff, Socket::READ_SIZE, 0);
			if (rbytes == Socket::READ_SIZE)
				message += buff;
			else if (rbytes < 0)
				throw SocketException("recv error", errno);
			else
			{
				message += buff;
				return message;
			}
		}
	}
}
