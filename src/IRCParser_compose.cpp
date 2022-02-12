/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_compose.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 09:35:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/11 11:35:54 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"
#include "../inc/IRCrpl.hpp"

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
		return IRC::ERR_NICKNAMEINUSE(*user, nickname);
	}
	if (user->has_mode("nick+"))
	{
		std::string old = user->get_name();
		user->make_nickname(nickname);
		return IRC::compose_message(nullptr, "NOTICE", *user, old + " is now known as " + nickname);
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
			if (user->has_mode("full+"))
				return IRC::ERR_TOOMANYCHANNELS(*user, chan);
			chan = database.add_channel(*chan_i);
			chan->add_user(*user);
			chan->add_oper(*user);
			user->add_channel(*chan);
		}
		else if (chan->has_mode("k+"))
		{
			if (not chan->check_pass(*pass_i))
			{
				user->send_message(IRC::ERR_PASSWDMISMATCH(*user));
				continue ;
			}
		}
		else
		{
			if (user->has_mode("full+"))
				return IRC::ERR_TOOMANYCHANNELS(*user, *chan);
			if (chan->has_mode("full+"))
				user->send_message(IRC::ERR_CHANNELISFULL(*user, *chan));
			chan->add_user(*user);
			user->add_channel(*chan);
		}

		std::cout << chan->get_users().size() << " users in channel " << chan->get_name() << "\n";
		chan->send_message(IRC::compose_message(user->compose(), "JOIN", "", chan->get_name(), false));
		IRCParser(":" + user->get_name() + " TOPIC " + chan->get_name()).exec(user->get_sock());

		Channel::user_container_type users = chan->get_users();
		std::string content;
		user->send_message(IRC::RPL_NAMREPLY(*user, '=', *chan, chan->get_users(), *chan));
		user->send_message(IRC::RPL_ENDOFNAMES(*user, *chan));
	}
	return "";
}

std::string
tlucanti::IRCParser::compose_mode()
{
	ITarget *tar;
	if (target.at(0) == '#' or target.at(0) == '&')
	{
		tar = database.get_channel(target);
		if (tar == nullptr)
			return IRC::ERR_NOSUCHNICK(*user, target, "channel with name");
	}
	else
	{
		tar = database[target];
		if (tar == nullptr)
			return IRC::ERR_NOSUCHNICK(*user, target, "user with nickname");
		if (user != tar)
			return IRC::ERR_USERSDONTMATCH(*user);
	}
	if (mode.size() > 2)
		return IRC::compose_message(nullptr, "MODE", *user, "you can change only "
			"one mode per command");

	if (has_suffix == 0)
		return IRC::RPL_UMODEIS(*user, user->get_modes());
	if (has_suffix == 1)
	{
		std::reverse(mode.begin(), mode.end());
		if (target.at(0) == '#' or target.at(0) == '&')
		{
			if (mode == "i+" or mode == "i-" or mode == "k-" or mode == "m+" or mode == "m-" or mode == "s+" or mode == "s-" or mode == "t+" or mode == "t-" or mode == "n+" or mode == "n-")
				user->make_mode(mode);
			else if (mode == "k+" or )
			else if (mode == "l+")
				return "";
		}
		else
		{
			if (mode == "i+" or mode == "i-" or mode == "o-")
				user->make_mode(mode);
			else if (mode == "o+")
				return "";
			else
				return IRC::ERR_UMODEUNKNOWNFLAG(*user, mode);
		}
	}
	else
		return "";
	return "";
}

std::string
tlucanti::IRCParser::compose_topic() const
{
	Channel *chan = database.get_channel(channel);

	if (chan == nullptr)
		return IRC::ERR_NOSUCHCHANNEL(*user, channel, "channel with name " +
			channel + " does not exist");
	if ((*chan)[*user] == nullptr)
		return IRC::ERR_NOTONCHANNEL(*user, *chan);
	if (not has_suffix)
	{
		if (chan->get_topic().empty())
			return IRC::RPL_NOTOPIC(*user, *chan);
		else
		{
			user->send_message(IRC::RPL_TOPIC(*user, *chan, chan->get_topic()));
			user->send_message(IRC::RPL_TOPICWHOTIME(*user, *chan, chan->get_topic_author(), chan->get_topic_time()));
			return "";
		}
	}
	else
	{
		if (chan->has_mode("t+") and not chan->is_oper(*user))
			return IRC::ERR_CHANOPRIVSNEEDED(*user, *chan);
		chan->make_topic(message, *user);
		chan->send_message(IRC::compose_message(user->compose(), "TOPIC", chan->get_name(), message));
		return "";
	}
}
