/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 12:09:27 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:44:02 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

const tlucanti::User tlucanti::User::nil = User(-1);
const char *tlucanti::User::modes = "iswo";

tlucanti::User::User(int sock) noexcept
		: sock_fd(sock), _has_pass(false), _has_nick(false), _has_user(false),
		_irc_operator(false) {}

void
tlucanti::User::make_user(const std::string &username,
	const std::string &hostname, const std::string &servername,
	const std::string &realname)
{
	_has_user = true;
	_username = username;
	_hostname = hostname;
	_servername = servername;
	_realname = realname;
}

void
tlucanti::User::make_nickname(const std::string &nickname)
{
	_has_nick = true;
	_nickname = nickname;
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
tlucanti::User::_check_perm__macro(const std::vector<std::string> &format) const
{
	for (std::vector<std::string>::const_iterator it=format.begin(); it != format.end(); ++it)
	{
		bool unknown = true;
		if (*it == "p-")
		{
			unknown = false;
			if (_has_pass)
				throw IRCParserException(IRCcodes::ERR_ALREADYREGISTRED, "you are already authorized");
		}
		if (*it == "p+" or *it == "n+" or *it == "u+")
		{
			unknown = false;
			if (not _has_pass)
				throw IRCParserException(IRCcodes::ERR_NOTREGISTERED, "you need to authenticate before use this command");
		}
		if (*it == "n+" or *it == "u+")
		{
			unknown = false;
			if (not _has_nick)
				throw IRCParserException(IRCcodes::ERR_NOTREGISTERED, "you need to identificate before use this command");
		}
		if (*it == "u+")
		{
			unknown = false;
			if (not _has_user)
				throw IRCParserException(IRCcodes::ERR_NOTREGISTERED, "you need to authorize before use this command");
		}
		if (unknown)
			throw IRCException("[internal error]", "invalid permission check flag", *it);
	}
}
