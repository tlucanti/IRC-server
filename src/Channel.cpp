/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:21:17 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:37 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

const char *tlucanti::Channel::modes = "opsitnmlvk";

tlucanti::Channel::Channel(const std::string &name)
		: name(name), is_nil(false) {}

void
tlucanti::Channel::add_user(const User &new_user)
{
	users.insert(new_user.get_sock());
}

void
tlucanti::Channel::add_oper(const User &new_oper)
{
	operators.insert(new_oper.get_sock());
}

void
tlucanti::Channel::assert_mode(const std::string &mode) const
{
	bool unknown = true;
	if (mode == "full+")
	{
		unknown = false;
		if (users.size() < tlucanti::channel_max_users)
			throw IRCParserException();
	}
	if (unknown)
		throw IRCException("[tlucanti::User::assert_mode__macro]", "invalid permission check flag", mode);
}

bool
tlucanti::Channel::has_mode(const std::string &mode) const
{
	try {
		assert_mode(mode);
		return true;
	} catch (IRCParserException &exc) {
		return false;
	}
}

void
tlucanti::Channel::make_pass(const std::string &_pass)
{
	pass = _pass;
}

bool
tlucanti::Channel::check_pass(const std::string &_pass) const
{
	if (pass.empty())
		return true;
	return _pass == pass;
}

void
tlucanti::Channel::remove_user(const User &del_user)
{
	users.erase(del_user.get_sock());
}

void
tlucanti::Channel::remove_oper(const User &del_oper)
{
	users.erase(del_oper.get_sock());
}

void
tlucanti::Channel::new_topic(const std::string &_topic)
{
	topic = _topic;
}

void
tlucanti::Channel::send_message(const std::string &message) const
{
	for (user_container_type::iterator it=users.begin(); it != users.end(); ++it)
	{
		Socket(*it).send(message);
	}
}
