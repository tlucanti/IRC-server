/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:33:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:02:55 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>

#include "defs.h"
#include "SocketException.hpp"

#ifndef SOCKET_HPP
# define SOCKET_HPP

namespace tlucanti
{
	class Socket
	{
	public:
		Socket(const std::string &address, int port);
		Socket &operator =(const Socket &cpy);

		Socket(int sock);

		__WUR int get_sock() const;
		static const int READ_SIZE;
	private:
		int _sock;
	};

	Socket accept(const Socket &_sock);
	std::string recv(const Socket &_sock);
}

#endif	// SOCKET_HPP
