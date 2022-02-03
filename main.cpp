/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 16:02:00 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:02:05 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Socket.hpp"
#include <poll.h>

#define CLI_NUM 10
int do_poll(tlucanti::Socket cli[CLI_NUM]);

const int tlucanti::Socket::READ_SIZE = 64;

int main()
{
	tlucanti::Socket clients[CLI_NUM];
	do_poll();
}

int do_poll(tlucanti::Socket cli[CLI_NUM])
{
	struct pollfd clients[CLI_NUM];

	for (int i=0; i < CLI_NUM; ++i)
	{
		clients[i].fd = cli[i].get_sock();
		clients[i].events = POLLIN; // POLLOUT
	}

	int status;
	while (true)
	{
		status = ::poll(reinterpret_cast<pollfd *>(&clients), CLI_NUM, 500);
		if (status == 0)
			continue ;
		else if (status < 0)
			throw std::runtime_error("poll error"); // change to normal exception
		else
		{
			for (int i=0; i < CLI_NUM; ++i)
			{
				if (clients[i].events == POLLIN)
				{
					clients[i].events = 0;
					std::cout << "data from client :<" << tlucanti::recv(cli[i]) << ">\n";
				}
			}
		}
	}
}

int _backend()
{
	tlucanti::Socket sock("0.0.0.0", 8080);
	while (true)
	{
		try
		{
			tlucanti::Socket client = tlucanti::accept(sock);
			std::string content = tlucanti::recv(client);
		}
		catch (std::exception &exc)
		{
			std::cout << exc.what() << std::endl;
			return 0;
		}
	}
}
