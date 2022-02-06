/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:38:53 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:38 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Database.hpp"

tlucanti::Database::~Database()
{
	for (fd_hashmap_type::iterator it=fd_access.begin(); it != fd_access.end(); ++it)
		delete it->second;
}

void
tlucanti::Database::add_client(const Socket &sock)
{
	User *new_user = new User(sock.get_sock());
	fd_access.insert({sock.get_sock(), new_user});
}

void
tlucanti::Database::remove_client(Socket &sock)
{
	User *cli = fd_access[sock.get_sock()];
	fd_access.erase(sock.get_sock());
	str_access.erase(cli->get_nickname());
	delete cli;
	sock.close();
}

__WUR bool
tlucanti::Database::make_edge(const std::string &nickname, const Socket &sock)
{
	if (str_access.find(nickname) != str_access.end())
		return true;
	str_access.insert({nickname, fd_access[sock.get_sock()]});
	return false;
}


__WUR tlucanti::User &
tlucanti::Database::operator [](int fd) const
{
	fd_hashmap_type::const_iterator it = fd_access.find(fd);
	if (it == fd_access.end())
		return const_cast<User &>(User::nil);
	return *it->second;
}

__WUR tlucanti::User &
tlucanti::Database::operator [](const std::string &nickname) const
{
	string_hashmap_type::const_iterator it = str_access.find(nickname);
	if (it == str_access.end())
		return const_cast<User &>(User::nil);
	return *it->second;
}

__WUR tlucanti::User &
tlucanti::Database::operator [](const Socket &client) const
{
	return (*this)[client.get_sock()];
}

__WUR tlucanti::Channel &
tlucanti::Database::get_channel(const std::string &channel) const
{
	channel_container_type::const_iterator it = cahnnels.find(channel);
	if (it == cahnnels.end())
		return const_cast<Channel &>(Channel::nil);
	return *it->second;
}
