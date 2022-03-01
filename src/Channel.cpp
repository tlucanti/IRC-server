/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:21:17 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/14 10:37:25 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

const char *tlucanti::Channel::modes = "likmstn";

tlucanti::Channel::Channel(const std::string &name) :
		name(name),
		topic_time(0),
		max_users(tlucanti::channel_max_users),
		_modes()
{
	create_time = time(nullptr);
	_modes.limit = true;
	_modes.inv = false;
	_modes.key = false;
	_modes.moder = false;
	_modes.secret = false;
	_modes.topic = false;
	_modes.next = true;
}

void
tlucanti::Channel::add_user(const ITarget &new_user)
{
	users.push_back(const_cast<ITarget *>(&new_user));
}

void
tlucanti::Channel::add_oper(const ITarget &new_oper)
{
	operators.insert(const_cast<ITarget *>(&new_oper));
}

void
tlucanti::Channel::add_voice(const ITarget &new_voice)
{
	voices.insert(const_cast<ITarget *>(&new_voice));
}

void
tlucanti::Channel::add_ban(const ITarget &new_ban)
{
	banned.insert(const_cast<ITarget *>(&new_ban));
}

void
tlucanti::Channel::remove_user(const ITarget &del_user)
{
	users.remove(const_cast<ITarget *>(&del_user));
}

void
tlucanti::Channel::remove_oper(const ITarget &del_oper)
{
	operators.erase(const_cast<ITarget *>(&del_oper));
	if (operators.empty() and not users.empty())
		operators.insert(*users.begin());
}

void
tlucanti::Channel::remove_voice(const ITarget &del_voive)
{
	voices.erase(const_cast<ITarget *>(&del_voive));
}

void
tlucanti::Channel::remove_ban(const ITarget &del_ban)
{
	banned.erase(const_cast<ITarget *>(&del_ban));
}

__WUR
bool
tlucanti::Channel::has_mode(const std::string &mode) const
{
	if (mode == "full+")
		return users.size() >= max_users;
	else if (mode == "full-")
		return  users.size() < max_users;
	else if (mode == "i+")
		return _modes.inv;
	else if (mode == "i-")
		return not _modes.inv;
	else if (mode == "k+")
		return _modes.key;
	else if (mode == "k-")
		return not _modes.key;
	else if (mode == "m+")
		return _modes.moder;
	else if (mode == "m-")
		return not _modes.moder;
	else if (mode == "s+")
		return _modes.secret;
	else if (mode == "s-")
		return not _modes.secret;
	else if (mode == "t+")
		return _modes.topic;
	else if (mode == "t-")
		return not _modes.topic;
	else if (mode == "n+")
		return _modes.next;
	else if (mode == "n-")
		return not _modes.next;
	else
		ABORT("invalid channel mode", mode);
}

void
tlucanti::Channel::make_mode(const std::string &mode)
{
	if (mode == "i+")
		_modes.inv = true;
	else if (mode == "i-")
		_modes.inv = false;
	else if (mode == "k+")
		_modes.key = true;
	else if (mode == "k-")
		_modes.key = false;
	else if (mode == "m+")
		_modes.moder = true;
	else if (mode == "m-")
		_modes.moder = false;
	else if (mode == "s+")
		_modes.secret = true;
	else if (mode == "s-")
		_modes.secret = false;
	else if (mode == "t+")
		_modes.topic = true;
	else if (mode == "t-")
		_modes.topic = false;
	else if (mode == "n+")
		_modes.next = true;
	else if (mode == "n-")
		_modes.next = false;
	else if (mode == "l+")
		return ;
	else // unknown
		ABORT("invalid channel mode flag", mode);
}

__WUR
std::string
tlucanti::Channel::get_modes() const
{
	std::string ret;
	if (_modes.limit)
		ret += 'l';
	if (_modes.inv)
		ret += 'i';
	if (_modes.key)
		ret += 'k';
	if (_modes.moder)
		ret += 'm';
	if (_modes.secret)
		ret += 's';
	if (_modes.topic)
		ret += 't';
	if (_modes.next)
		ret += 'n';
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

__WUR
bool
tlucanti::Channel::is_oper(const ITarget &target) const
{
	if (operators.find(const_cast<ITarget *>(&target)) == operators.end())
		return false;
	return true;
}

__WUR
bool
tlucanti::Channel::is_voice(const ITarget &target) const
{
	if (voices.find(const_cast<ITarget *>(&target)) == voices.end())
		return false;
	return true;
}

__WUR
bool
tlucanti::Channel::is_banned(const ITarget &target) const
{
	if (banned.find(const_cast<ITarget *>(&target)) == banned.end())
		return false;
	return true;
}

void
tlucanti::Channel::send_message(const std::string &message) const
{
	user_container_type::const_iterator it=users.begin();
	for (; it != users.end(); ++it)
		(*it)->send_message(message);
}

void
tlucanti::Channel::send_message(const std::string &message, const ITarget &except) const
{
	user_container_type::const_iterator it=users.begin();
	for (; it != users.end(); ++it)
	{
		if ((*it)->get_name() == except.get_name())
			continue ;
		(*it)->send_message(message);
	}
}

void
tlucanti::Channel::make_topic(const std::string &_topic,
	const ITarget &author)
{
	topic = _topic;
	topic_author = author.get_name();
	topic_time = std::time(nullptr);
}

__WUR
tlucanti::ITarget *
tlucanti::Channel::operator [](const std::string &user) const
{
	user_container_type::const_iterator it = users.begin();
	for (; it != users.end(); ++it)
	{
		if ((*it)->get_name() == user)
			return *it;
	}
	return nullptr;
}

__WUR
tlucanti::ITarget *
tlucanti::Channel::operator [](const ITarget &user) const
{
	return (*this)[user.get_name()];
}

std::ostream &
tlucanti::operator <<(std::ostream &out, const tlucanti::Channel &chan)
{
	out << chan.get_name();
	return out;
}
