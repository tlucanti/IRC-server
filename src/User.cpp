/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 12:09:27 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/13 17:32:47 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

const char *tlucanti::User::modes = "iswo";

tlucanti::User::User(const Socket &_sock) noexcept
		: sock(_sock), _modes {}
{
	_modes.pass = false;
	_modes.nick = false;
	_modes.reg = false;
	_modes.oper = false;
}

void
tlucanti::User::make_nickname(const std::string &nickname)
{
	_modes.nick = true;
	_nickname = nickname;
}

void
tlucanti::User::make_user(const std::string &username, const std::string &realname)
{
	_modes.reg = true;
	_username = username;
	_realname = realname;
}

__WUR
std::string
tlucanti::User::compose() const
{
	return _nickname + '!' + _username + '@' + tlucanti::server_address;
}

void
tlucanti::User::send_message(const std::string &message) const
{
	sock.send(message);
}

void
tlucanti::User::send_to_chanels(const std::string &message) const
{
	channels_list::iterator it=channels_member.begin();
	for (; it != channels_member.end(); ++it)
		(*it)->send_message(message);
}

void
tlucanti::User::add_channel(Channel &chan)
{
	channels_member.insert(&chan);
}

void
tlucanti::User::remove_channel(Channel &chan)
{
	channels_member.erase(&chan);
}

void 
tlucanti::User::assert_mode(const std::string &mode) const
{
	bool unknown = true;
	if (mode == "pass-")
	{
		unknown = false;
		if (_modes.pass)
			throw IRCParserException(IRC::ERR_ALREADYREGISTERED(*this));
	}
	if (mode == "pass+" or mode == "nick+" or mode == "reg+")
	{
		unknown = false;
		if (not _modes.pass)
			throw IRCParserException(IRC::ERR_NOTREGISTERED(*this));
	}
	if (mode == "nick+" or mode == "reg+")
	{
		unknown = false;
		if (not _modes.nick)
			throw IRCParserException(IRC::ERR_NOTREGISTERED(*this));
	}
	if (mode == "reg+")
	{
		unknown = false;
		if (not _modes.reg)
			throw IRCParserException(IRC::ERR_NOTREGISTERED(*this));
	}
	if (mode == "reg-")
	{
		unknown = false;
		if (_modes.reg)
			throw IRCParserException(IRC::ERR_ALREADYREGISTERED(*this));
	}
	if (mode == "full-")
	{
		unknown = false;
		if (channels_member.size() >= tlucanti::user_max_channels)
			throw tlucanti::IRCParserException();
	}
	if (mode == "full+")
	{
		unknown = false;
		if (channels_member.size() < tlucanti::user_max_channels)
			throw tlucanti::IRCParserException();
	}
	if (mode == "o+")
	{
		unknown = false;
		if (not _modes.oper)
			throw tlucanti::IRCParserException();
	}
	if (unknown)
		ABORT("invalid user mode", mode);
}

__WUR
bool
tlucanti::User::has_mode(const std::string &mode) const
{
	try {
		assert_mode(mode);
		return true;
	} catch (IRCParserException &exc) {
		return false;
	}
}

void
tlucanti::User::make_mode(const std::string &mode)
{
	bool unknown = true;
	if (mode == "pass+")
	{
		unknown = false;
		_modes.pass = true;
	}
	else if (mode == "o+")
	{
		unknown = false;
		_modes.oper = true;
	}
	if (unknown)
		ABORT("invalid user mode flag", mode);
}

__WUR
std::string
tlucanti::User::get_modes() const
{
	std::string ret = "+";
	if (_modes.reg)
		ret += 'r';
	if (_modes.oper)
		ret += 'o';
	if (_modes.inv)
		ret += 'i';
	return ret;
}

std::ostream &
tlucanti::operator <<(std::ostream &out, const tlucanti::User &usr)
{
	if (usr.get_name().empty())
		out << '*';
	else
		out << usr.get_name();
	return out;
}
