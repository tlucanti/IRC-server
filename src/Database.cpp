/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:38:53 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/16 19:56:03 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Database.hpp"

tlucanti::Database::~Database()
{
	sock_hashmap_type::iterator it=sock_access.begin();
	for (; it != sock_access.end(); ++it)
		delete it->second;
}

void
tlucanti::Database::collapse()
{
	sock_hashmap_type::const_iterator it = sock_access.begin();
	for (; it != sock_access.end(); ++it)
		remove_client(*it->second);
}

tlucanti::Channel *
tlucanti::Database::add_channel(const std::string &name)
{
	Channel *new_chan = new Channel(name);
	channels[name] = new_chan;
	return new_chan;
}

void
tlucanti::Database::add_client(const Socket &_sock)
{
	User *new_user = new User(_sock);
	std::pair<int, User *> add(_sock.get_sock(), new_user);
	sock_access.insert(add);
}

void
tlucanti::Database::remove_client(User &user)
{
	sock_access.erase(user.get_sock().get_sock());
	str_access.erase(user.get_name());
	const_cast<Socket &>(user.get_sock()).close();
	const User::channels_list user_channels = user.get_channels();
	User::channels_list::const_iterator it = user_channels.begin();
	for (; it != user_channels.end(); ++it)
	{
		(*it)->remove_user(user);
		(*it)->remove_voice(user);
		(*it)->remove_oper(user);
		if ((*it)->get_users().empty()) // remove empty channel
			channels.erase((*it)->get_name());
	}
	delete (&user);
}

__WUR bool
tlucanti::Database::make_edge(const std::string &nickname, const Socket &sock)
{
	if (str_access.find(nickname) != str_access.end())
		return true;
	str_access.insert({nickname, sock_access[sock.get_sock()]});
	return false;
}

void
tlucanti::Database::make_invite(const User &user, const Channel &channel)
{
	time_t expiration = time(nullptr) + 60;
	InviteNode invitation = InviteNode(&user, &channel, expiration);
	invite_table.insert(invitation);
}

void
tlucanti::Database::remove_invite(const User &user, const Channel &channel)
{
	InviteNode invitation = InviteNode(&user, &channel, 0);
	invite_table_type::iterator inv = invite_table.find(invitation);
	if (inv != invite_table.end())
		invite_table.erase(inv);
}

__WUR
bool
tlucanti::Database::has_invite(const User &user, const Channel &channel)
{
	InviteNode invitation = InviteNode(&user, &channel, 0);
	return invite_table.find(invitation) == invite_table.end();
}

void
tlucanti::Database::send_to_all(const std::string &message) const
{
	sock_hashmap_type::const_iterator it = sock_access.begin();
	for (; it != sock_access.end(); ++it)
		it->second->send_message(message);
}

__WUR tlucanti::User *
tlucanti::Database::operator [](const std::string &nickname) const
{
	string_hashmap_type::const_iterator it = str_access.find(nickname);
	if (it == str_access.end())
		return nullptr;
	return it->second;
}

__WUR tlucanti::User *
tlucanti::Database::operator [](const Socket &client) const
{
	sock_hashmap_type::const_iterator it = sock_access.find(client.get_sock());
	if (it == sock_access.end())
		return nullptr;
	return it->second;
}

__WUR tlucanti::Channel *
tlucanti::Database::get_channel(const std::string &channel) const
{
	channel_container_type::const_iterator it = channels.find(channel);
	if (it == channels.end())
		return nullptr;
	return it->second;
}
