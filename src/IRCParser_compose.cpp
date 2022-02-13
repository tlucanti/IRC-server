/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_compose.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 09:35:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/13 21:28:09 by tlucanti         ###   ########.fr       */
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
	user->make_user(nickname, realname);
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
	user->send_message(IRC::compose_message(*user, "MODE", *user, user->get_modes()));
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
	else // invalid password
		return IRC::ERR_PASSWDMISMATCH(*user);
}

std::string
tlucanti::IRCParser::compose_quit()
{
	user->assert_mode("reg+");
	if (message.empty())
		message = "Client Quit";
	user->send_to_chanels(IRC::compose_message(user->compose(), "QUIT", "", message));
	user->send_message(IRC::ERROR(message));
	database.remove_client(*user);
	return "";
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
		else // channel has no password
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
	bool chan_mode = target.at(0) == '#' or target.at(0) == '&';

	user->assert_mode("reg+");
	if (chan_mode)
	{
		tar = database.get_channel(target);
		if (tar == nullptr)
			return IRC::ERR_NOSUCHNICK(*user, target, "channel with name");
	}
	else // user mode
	{
		tar = database[target];
		if (tar == nullptr)
			return IRC::ERR_NOSUCHNICK(*user, target, "user with nickname");
		if (user != tar)
		{
			if (has_suffix == 0)
				return IRC::ERR_USERSDONTMATCH(*user, "see");
			else // change mode
				return IRC::ERR_USERSDONTMATCH(*user, "change");
		}
	}
	if (mode.size() > 2)
		return IRC::compose_message(nullptr, "MODE", *user, "you can change only "
			"one mode per command");

	if (has_suffix == 0)
	{
		if (not chan_mode)
			return IRC::RPL_UMODEIS(*tar, tar->get_modes());
		user->send_message(IRC::RPL_CHANNELMODEIS(*user, *tar, tar->get_modes(), ""));
		user->send_message(IRC::RPL_CREATIONTIME(*user, *tar, dynamic_cast<Channel *>(tar)->get_create_time()));
		return "";
	}
	if (chan_mode and not dynamic_cast<Channel *>(tar)->is_oper(*user))
		return IRC::ERR_CHANOPRIVSNEEDED(*user, *tar);
	std::reverse(mode.begin(), mode.end());
	if (chan_mode)
	{
		Channel *chan = dynamic_cast<Channel *>(tar);
		if (mode == "i+" or mode == "i-" or mode == "k-" or mode == "m+" or mode == "m-" or mode == "s+" or mode == "s-" or mode == "t+" or mode == "t-" or mode == "n+" or mode == "n-" or mode == "l+")
		{
			if (has_suffix > 1)
				user->send_message(IRC::ERR_INVALIDMODEPARAM(*user, *chan, mode.at(0), truncate(message, 10), "extra tokens ignored"));
			chan->make_mode(mode);
		}
		else if (mode == "k+")
		{
			if (has_suffix == 1)
				return IRC::ERR_NEEDMOREPARAMS(*user, "MODE", "expected channel password");
			chan->make_pass(message);
			chan->make_mode("k+");
		}
		else if (mode == "v+" or mode == "o+" or mode == "v-" or mode == "o-")
		{
			if (has_suffix == 1)
				return IRC::ERR_NEEDMOREPARAMS(*user, "MODE", "expected username");
			User *recipient = dynamic_cast<User *>((*chan)[message]);
			if (recipient == nullptr)
				return IRC::ERR_INVALIDMODEPARAM(*user, *chan, mode.at(0), target, "user not in channel");
			if (mode == "v+")
			{
				if (chan->is_voice(*recipient))
					return IRC::ERR_INVALIDMODEPARAM(*user, *recipient, mode.at(0), "+v", "user already has voice");
				chan->add_voice(*recipient);
			}
			else if (mode == "o+")
			{
				if (chan->is_oper(*recipient))
					return IRC::ERR_INVALIDMODEPARAM(*user, *recipient, mode.at(0), "+o", "user is already server operator");
				chan->add_oper(*recipient);
			}
			else if (mode == "v-")
			{
				if (not chan->is_voice(*recipient))
					return IRC::ERR_INVALIDMODEPARAM(*user, *recipient, mode.at(0), "-v", "user has not voice");
				chan->remove_voice(*recipient);
			}
			else if (mode == "o-")
			{
				if (not chan->is_oper(*recipient))
					return IRC::ERR_INVALIDMODEPARAM(*user, *recipient, mode.at(0), "-o", "user is not channel operator");
				chan->remove_oper(*recipient);
			}
			else // error
				ABORT("invalid mode", mode);
		}
		else
			return IRC::ERR_UMODEUNKNOWNFLAG(*user, mode);
	}
	else // user mode
	{
		if (mode == "i+" or mode == "i-" or mode == "o-")
			user->make_mode(mode);
		else if (mode == "o+")
			return "";
		else
			return IRC::ERR_UMODEUNKNOWNFLAG(*user, mode);
	}
	return IRC::compose_message(user->compose(), "MODE", *user, tar->get_name() + ' ' + mode + ' ' + user->get_name());

}

std::string
tlucanti::IRCParser::compose_privmsg() const
{
	user->assert_mode("reg+");
	for (arg_list_type::const_iterator it=user_list.begin();
		it != user_list.end(); ++it)
	{
		User *cli = database[*it];
		if (cli == nullptr)
			throw IRCParserException(IRC::ERR_NOSUCHNICK(*user, *it, "user with nickname"));
		cli->send_message(IRC::RPL_AWAY(*user, *cli, message));
	}
	for (arg_list_type::const_iterator it=chan_list.begin();
		it != chan_list.end(); ++it)
	{
		Channel *chan = database.get_channel(*it);
		if (chan == nullptr)
			throw IRCParserException(IRC::ERR_NOSUCHNICK(*user, *it, "channel with name"));
		chan->send_message(message);
	}
	return "";
}

std::string
tlucanti::IRCParser::compose_who() const
{
	bool chan_who = target.at(0) == '#' or target.at(0) == '&';

	user->assert_mode("reg+");
	if (chan_who)
	{
		Channel *chan = database.get_channel(target);
		if (chan == nullptr)
			return IRC::ERR_NOSUCHNICK(*user, target, "channel with name");
		const Channel::user_container_type users = chan->get_users();
		Channel::user_container_type::const_iterator it = users.begin();
		for (; it != users.end(); ++it)
			user->send_message(IRC::RPL_WHOREPLY(*user, dynamic_cast<const User &>(**it), *chan));
	}
	else // user mode
	{
		ITarget *tar = database[target];
		if (tar == nullptr)
			return IRC::ERR_NOSUCHNICK(*user, target, "user with nickname");
		user->send_message(IRC::RPL_WHOREPLY(*user, dynamic_cast<const User &>(*tar), '*'));
	}
	return IRC::RPL_ENDOFWHO(*user, target);
}

std::string
tlucanti::IRCParser::compose_topic() const
{
	Channel *chan = database.get_channel(channel);

	user->assert_mode("reg+");
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
