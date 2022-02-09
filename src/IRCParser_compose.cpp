/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_compose.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 09:35:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/09 15:42:23 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

namespace tlucanti
{
	extern Database database;
}

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
	user->assert_mode("reg-");
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
	{
		if (user->has_mode("o+"))
			return IRC::RPL_YOUREOPER(*user ,"you are already operator");
		user->make_mode("o+");
		++database.operators_cnt;
		return IRC::RPL_YOUREOPER(*user, "be careful with this power");
	}
	else
		return IRC::ERR_PASSWDMISMATCH(*user);
}

std::string
tlucanti::IRCParser::compose_quit() const
{
	user->assert_mode("reg+");
	throw IRCException("[tlucanti::IRCParser::compose_quit]", "implement quit");
}

std::string
tlucanti::IRCParser::compose_join()
{
	user->assert_mode("reg+");
	arg_list_type::iterator pass_i = pass_list.begin();
	arg_list_type::iterator chan_i = chan_list.begin();

	if (pass_list.size() > chan_list.size())
		return IRC::compose_message(nullptr, "NOTICE", *user, "password tokens at the end of command");
	pass_list.resize(chan_list.size());
	for (; chan_i != chan_list.end(); ++chan_i, ++pass_i)
	{
		Channel *chan = database.get_channel(*chan_i);
		if (chan == nullptr)
		{
			user->assert_mode("full-");
			chan = database.add_channel(*chan_i);
			chan->add_user(*user);
			chan->add_oper(*user);
			user->add_channel(*chan);
			continue ;
		}
		else if (chan->has_mode("k+"))
		{
			if (not chan->check_pass(*pass_i))
			{
				user->send_message(IRC::ERR_PASSWDMISMATCH(*user));
				continue ;
			}
		}
		if (user->has_mode("full+"))
			return IRC::ERR_TOOMANYCHANNELS(*user, *chan);
		if (chan->has_mode("full+"))
			return IRC::ERR_CHANNELISFULL(*user, *chan);
		chan->assert_mode("full-");
		chan->add_user(*user);
		user->add_channel(*chan);
		chan->send_message(IRC::compose_message(*user, "JOIN", "", chan->get_name(), false));
		IRCParser(":" + user->get_nickname() + ' ' + chan->get_name()).exec(user->get_sock());

		Channel::user_container_type users = chan->get_users();
		int enough = 0;
		std::string content;
		user->send_message(IRC::RPL_NAMREPLY(*user, '=', *chan, chan->get_users()));
		user->send_message(IRC::RPL_ENDOFNAMES(*user, *chan));
	}
}
