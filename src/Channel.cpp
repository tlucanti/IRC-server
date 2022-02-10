/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:21:17 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/10 22:03:10 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

const char *tlucanti::Channel::modes = "opsitnmlvk";

tlucanti::Channel::Channel(const std::string &name)
		: name(name)
{
	_modes.key = false;
}

void
tlucanti::Channel::add_user(const ITarget &new_user)
{
	users.push_back(const_cast<ITarget *>(&new_user));
}

void
tlucanti::Channel::add_oper(const ITarget &new_oper)
{
	operators.push_back(const_cast<ITarget *>(&new_oper));
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
	if (mode == "full-")
	{
		unknown = false;
		if (users.size() >= tlucanti::channel_max_users)
			throw IRCParserException();
	}
	if (mode == "k+")
	{
		unknown = false;
		if (not _modes.key)
			throw IRCParserException();
	}
	if (unknown)
		throw IRCException("[tlucanti::Channel::assert_mode]", "invalid channel mode", mode);
}

__WUR
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
tlucanti::Channel::make_mode(const std::string &mode)
{
	bool unknown = true;
	if (unknown)
		throw IRCException("[tlucanti::User::make_mode__macro]", "invalid mode flag", mode);
}

__WUR
std::string
tlucanti::Channel::get_modes() const
{
	std::string ret;
	if (_modes.key)
		ret += 'k';
	if (not ret.empty())
		ret = '+' + ret;
	return ret;
}

void
tlucanti::Channel::make_pass(const std::string &_pass)
{
	pass = _pass;
}

__WUR
bool
tlucanti::Channel::check_pass(const std::string &_pass) const
{
	if (pass.empty())
		return true;
	return _pass == pass;
}

void
tlucanti::Channel::remove_user(const ITarget &del_user)
{
	users.remove(const_cast<ITarget *>(&del_user));
}

void
tlucanti::Channel::remove_oper(const ITarget &del_oper)
{
	users.remove(const_cast<ITarget *>(&del_oper));
}

void
tlucanti::Channel::new_topic(const std::string &_topic)
{
	topic = _topic;
}

void
tlucanti::Channel::send_message(const std::string &message) const
{
	for (user_container_type::const_iterator it=users.begin(); it != users.end(); ++it)
		(*it)->send_message(message);
}

std::ostream &
tlucanti::operator <<(std::ostream &out, const tlucanti::Channel &chan)
{
	out << chan.get_name();
	return out;
}
