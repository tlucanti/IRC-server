/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_compose.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 09:35:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/08 18:32:21 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

std::string
tlucanti::IRCParser::compose_cap() const
{
	return "";
}

std::string
tlucanti::IRCParser::compose_pass() const
{
	user->assert_mode("pass-");
	if (password != tlucanti::server_password)
		return IRC::compose_message(nullptr, "NOTICE", "*", "password incorrect");
	user->make_mode("pass+");
	return "";
}

std::string
tlucanti::IRCParser::compose_nick() const
{
	user->assert_mode("pass+");
	if (database.make_edge(nickname, user->get_sock()))
	{
		if (user->has_mode("nick+"))
			return IRC::ERR_NICKNAMEINUSE('*', nickname);
		return IRC::ERR_NICKNAMEINUSE(user, nickname);
	}
	if (user->has_mode("nick+"))
	{
		std::string old = user->get_nickname();
		user->make_nickname(nickname);
		return IRC::compose_message(nullptr, "NOTICE", user, old + " is now known as " + nickname);
	}
	user->make_nickname(nickname);
	return "";
}

std::string
tlucanti::IRCParser::compose_user() const
{
	user->assert_mode("nick+");
	user->make_user(nickname, hostname, servername, realname);
	user->send_message(IRC::RPL_WELCOME(*user));
	user->send_message(IRC::RPL_YOURHOST(*user));
	user->send_message(IRC::RPL_CREATED(*user));
	user->send_message(IRC::RPL_MYINFO(*user, User::modes, Channel::modes));
	user->send_message(IRC::RPL_ISUPPORT(*user));
	user->send_message(IRC::RPL_LUSERCLIENT(*user, database.get_user_cnt(), database.invisible_users, database.get_chan_cnt()));
	user->send_message(IRC::RPL_LUSEROP(*user, database.operators_cnt));
	user->send_message(IRC::RPL_LUSERCHANNELS(*user, database.get_chan_cnt()));
	user->send_message(IRC::RPL_LUSERME(*user, database.get_user_cnt(), 1));
	user->send_message(IRC::RPL_LOCALUSERS(*user, database.get_user_cnt(), database.max_users));
	user->send_message(IRC::RPL_GLOBALUSERS(*user, database.get_user_cnt(), database.max_users));
	user->send_message(IRC::RPL_MOTDSTART(*user));
	user->send_message(IRC::RPL_MOTD(*user, "../MOTD.txt"));
	user->send_message(IRC::RPL_ENDOFMOTD(*user));
	return "";
}

std::string
tlucanti::IRCParser::compose_oper() const
{
	user->assert_mode("reg+");
	if (nickname == tlucanti::server_oper_login and password == tlucanti::server_oper_password)
		user->make_mode("o+");
	else
		return IRC::ERR_PASSWDMISMATCH(*user);
	return "";
}
