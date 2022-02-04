/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:48:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:03:24 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

tlucanti::Socket tlucanti::Socket::nil = tlucanti::Socket(-1);

tlucanti::Socket::Socket(const std::string &address, uint16_t port)
{
	struct sockaddr_in _addr = {};

	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == -1)
		throw SocketException("cannot create socket", errno);
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = inet_addr(address.c_str());
	_addr.sin_port = htons(port);
	if (bind(_sock, reinterpret_cast<const sockaddr *>(&_addr), sizeof(_addr)))
		throw SocketException("cannot bind address", errno);
	if (listen(_sock, 10))
		throw SocketException("cannot listen port", errno);
}

tlucanti::Socket::~Socket() noexcept
{
	close(_sock);
}

tlucanti::Socket::Socket(int sock) noexcept
		: _sock(sock) {}

tlucanti::Socket &
tlucanti::Socket::operator =(const Socket &cpy)
{
	_sock = cpy._sock;
	return *this;
}
