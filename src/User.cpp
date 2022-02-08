/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 12:09:27 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/08 22:18:09 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"
#include "../inc/Database.hpp"

const char *tlucanti::User::modes = "iswo";

namespace tlucanti {
	extern tlucanti::Database database;
}

tlucanti::User::User(int sock) noexcept
		: sock_fd(sock)
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
tlucanti::User::make_user(const std::string &username,
	const std::string &hostname, const std::string &servername,
	const std::string &realname)
{
	_modes.reg = true;
	_username = username;
	_hostname = hostname;
	_servername = servername;
	_realname = realname;
}

std::string
tlucanti::User::compose() const
{
	return _nickname + '!' + _username + '@' + tlucanti::server_address;
}

void
tlucanti::User::send_message(const std::string &message) const
{
	Socket(sock_fd).send(message);
}


void 
tlucanti::User::assert_mode__macro(const std::vector<std::string> &format) const
{
	for (std::vector<std::string>::const_iterator it=format.begin(); it != format.end(); ++it)
	{
		bool unknown = true;
		if (*it == "pass-")
		{
			unknown = false;
			if (_modes.pass)
				throw IRCParserException(IRC::ERR_ALREADYREGISTERED(*this));
		}
		if (*it == "pass+" or *it == "nick+" or *it == "reg+")
		{
			unknown = false;
			if (not _modes.pass)
				throw IRCParserException(IRC::ERR_NOTREGISTERED(*this));
		}
		if (*it == "nick+" or *it == "reg+")
		{
			unknown = false;
			if (not _modes.nick)
				throw IRCParserException(IRC::ERR_NOTREGISTERED(*this));
		}
		if (*it == "reg+")
		{
			unknown = false;
			if (not _modes.reg)
				throw IRCParserException(IRC::ERR_NOTREGISTERED(*this));
		}
		if (unknown)
			throw IRCException("[tlucanti::User::assert_mode__macro]", "invalid permission check flag", *it);
	}
}

bool
tlucanti::User::has_mode__macro(const std::vector<std::string> &format) const
{
	try {
		assert_mode__macro(format);
		return true;
	} catch (IRCParserException &exc) {
		return false;
	}
}

void
tlucanti::User::make_mode__macro(const std::vector<std::string> &format)
{
	for (std::vector<std::string>::const_iterator it=format.begin(); it != format.end(); ++it)
	{
		bool unknown = true;
		if (*it == "pass+")
		{
			unknown = false;
			_modes.pass = true;
		}
		else if (*it == "o+")
		{
			unknown = false;
			_modes.oper = true;
			++database.operators_cnt;
		}
		if (unknown)
			throw IRCException("[tlucanti::User::make_mode__macro]", "invalid permission check flag", *it);
	}
}

std::ostream &
operator <<(std::ostream &out, const tlucanti::User &usr)
{
	if (usr.get_nickname().empty())
		out << '*';
	else
		out << usr.get_nickname();
	return out;
}
