/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 11:34:11 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/04 13:39:26 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

tlucanti::Server::Server(const std::string &address, uint16_t port)
		: cli_cnt(0), _polls_unprocessed(0), _last_processed_poll(-1), sock(address, port) {}

tlucanti::Server::~Server() noexcept
{
	for (container::iterator it = poll_data.begin(); it != poll_data.end(); ++it)
		close(it->fd);
}

__WUR tlucanti::Socket
tlucanti::Server::accept()
{
	return tlucanti::accept(sock);
}

__WUR tlucanti::Socket
tlucanti::Server::poll()
{
	if (_polls_unprocessed == 0)
		/*
			if all clients were processed - call ::poll again and return first client with event
			else - continue from next client with event
	 	*/
	{
		_polls_unprocessed = ::poll(poll_data.data(), cli_cnt, WAIT_TIME);
		if (_polls_unprocessed < 0)
			throw ServerException("poll error", errno);
		else if (_polls_unprocessed == 0)
		{
			_last_processed_poll = -1;
			return Socket::nil;
		}
	}
	for (int cli=_last_processed_poll + 1; cli < cli_cnt; ++cli)
	{
		if (poll_data.at(cli).events & POLLIN)
		{
			poll_data[cli].events = 0; // don't need .at because already checked
			--_polls_unprocessed;
			return Socket(poll_data[cli].fd);
		}
	}
	_polls_unprocessed = 0;
	_last_processed_poll = -1;
	return Socket::nil;
}

void
tlucanti::Server::add_client(Socket &new_cli)
{
	struct pollfd new_struct {};
	new_struct.fd = new_cli.get_sock();
	new_struct.events = POLLIN;
	poll_data.push_back(new_struct);
}

//__WUR inline size_t
//tlucanti::Server::size() const
//{
//	return _size;
//}
//
//__WUR inline struct pollfd *
//tlucanti::Server::get_poll_data() const
//{
//	return const_cast<struct pollfd *>(poll_data.data());
//}