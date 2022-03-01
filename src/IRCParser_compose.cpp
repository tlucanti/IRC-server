/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_compose.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 09:35:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/23 20:25:53 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

namespace tlucanti {
	extern Database database;
}

// ---------------------------- Channel Operations -----------------------------
__WUR
std::string
tlucanti::IRCParser::compose_cap() const
{
	return "OK";
}

__WUR
std::string
tlucanti::IRCParser::compose_pass() const
{
	user->assert_mode("pass-");
	if (password != tlucanti::server_password)
		return IRC::ERR_PASSWDMISMATCH('*');
	user->make_mode("pass+");
	return "OK";
}

__WUR
std::string
tlucanti::IRCParser::compose_nick() const
{
	user->assert_mode("pass+");
	if (nickname.size() > tlucanti::user_max_nick_len)
		return IRC::ERR_ERRONEUSNICKNAME(*user, true);
	if (user->has_mode("nick+") and user->get_name() == nickname)
		return "OK";
	if (database.make_edge(nickname, user->get_sock()))
	{
		if (user->has_mode("nick+"))
			return IRC::ERR_NICKNAMEINUSE('*', nickname);
		return IRC::ERR_NICKNAMEINUSE(*user, nickname);
	}
	if (user->has_mode("nick+"))
	{
		database.remove_edge(user->get_name());
		std::string old = user->compose();
		user->make_nickname(nickname);
		user->send_to_channels(IRC::compose_message(old, "NICK", nullptr, nickname));
	}
	else
		user->make_nickname(nickname);
	return "OK";
}

__WUR
std::string
tlucanti::IRCParser::compose_user() const
{
	user->assert_mode("nick+");
	user->assert_mode("reg-");
	user->make_user(nickname, realname);
	user->do_ping();
	return "OK";
}

__WUR
std::string
tlucanti::IRCParser::
compose_ping() const
{
	user->assert_mode("reg+");
	if (not has_preffix)
		return "OK";
	return IRC::compose_message(nullptr, "PONG", tlucanti::server_name, message);
}

__WUR
std::string
tlucanti::IRCParser::compose_pong() const
{
	if (user->has_mode("ping+"))
	{
		if (user->check_ping(message))
			user->reset_ping();
		user->assert_mode("ping-");
	}
	if (user->reg_waiting())
	{
		user->send_message(IRC::RPL_WELCOME(*user));
		user->send_message(IRC::RPL_YOURHOST(*user));
		user->send_message(IRC::RPL_CREATED(*user));
		user->send_message(IRC::RPL_MYINFO(*user, User::modes, Channel::modes));
		user->send_message(compose_version());
		user->send_message(IRC::RPL_LUSERCLIENT(*user, database.get_user_cnt(), database.invisible_users, database.get_chan_cnt()));
		user->send_message(IRC::RPL_LUSEROP(*user, database.operators_cnt));
		user->send_message(IRC::RPL_LUSERCHANNELS(*user, database.get_chan_cnt()));
		user->send_message(IRC::RPL_LUSERME(*user, database.get_user_cnt(), 1));
		user->send_message(IRC::RPL_LOCALUSERS(*user, database.get_user_cnt(), database.max_users));
		user->send_message(IRC::RPL_GLOBALUSERS(*user, database.get_user_cnt(), database.max_users));
		user->send_message(compose_motd());
		user->send_message(IRC::compose_message(user->compose(), "MODE", *user, user->get_modes()));
		user->complete_user();
	}
	return "OK";
}

__WUR
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

__WUR
std::string
tlucanti::IRCParser::compose_quit()
{
	if (message.empty())
		message = "Client Quit";
	user->send_to_channels(IRC::compose_message(user->compose(), "QUIT", nullptr, message));
	user->send_message(IRC::ERROR(message));
	database.remove_client(*user);
	return "";
}

// ---------------------------- Channel Operations -----------------------------
__WUR
std::string
tlucanti::IRCParser::compose_join()
{
	user->assert_mode("reg+");
	while (chan_list.size() > pass_list.size())
		pass_list.push_back("");
	arg_list_type::iterator chan_i = chan_list.begin();
	arg_list_type::iterator pass_i = pass_list.begin();

	if (pass_list.size() > chan_list.size())
		return IRC::compose_message(nullptr, "NOTICE", *user, "extra password tokens at the end of command");

	for (; chan_i != chan_list.end(); ++chan_i, ++pass_i)
	{
		Channel *chan = database.get_channel(*chan_i);

		if (chan != nullptr and (*chan)[*user] != nullptr) // already in channel
			return "OK";
		if (user->has_mode("full+"))
			return IRC::ERR_TOOMANYCHANNELS(*user, channel);
		if (chan == nullptr) // create new channel
		{
			chan = database.add_channel(*chan_i);
			chan->add_oper(*user);
		}
		if (chan->is_banned(*user)) // banned from channel
		{
			user->send_message(IRC::ERR_BANNEDFROMCHAN(*user, *chan));
			continue ;
		}	
		if (chan->has_mode("i+") and not database.has_invite(*user, *chan)) // invite only channel
		{
			user->send_message(IRC::ERR_INVITEONLYCHAN(*user, *chan));
			continue ;
		}
		if (chan->has_mode("k+") and not chan->check_pass(*pass_i)) // channel has password
		{
			user->send_message(IRC::ERR_BADCHANNELKEY(*user, *chan));
			continue ;
		}
		if (chan->has_mode("full+"))
		{
			user->send_message(IRC::ERR_CHANNELISFULL(*user, *chan));
			continue ;
		}

		database.remove_invite(*user, *chan);
		chan->add_user(*user);
		user->add_channel(*chan);

		chan->send_message(IRC::compose_message(user->compose(), "JOIN", nullptr, chan->get_name(), false));
		channel = chan->get_name();
		user->send_message(compose_topic());

		Channel::user_container_type users = chan->get_users();
		std::string content;
		user->send_message(IRC::RPL_NAMREPLY(*user, '=', *chan, chan->get_users(), *chan));
		user->send_message(IRC::RPL_ENDOFNAMES(*user, *chan));
	}
	return "OK";
}

__WUR
std::string
tlucanti::IRCParser::compose_part() const
{
	arg_list_type::const_iterator it = chan_list.begin();

	user->assert_mode("reg+");
	for (; it != chan_list.end(); ++it)
	{
		Channel *chan = database.get_channel(*it);

		if (chan == nullptr)
			user->send_message(IRC::ERR_NOSUCHCHANNEL(*user, *it, "channel with name"));
		else if ((*chan)[*user] == nullptr)
			user->send_message(IRC::ERR_NOTONCHANNEL(*user, *chan));
		else
		{
			chan->send_message(IRC::compose_message(user->compose(), "PART", *chan, message));
			user->remove_channel(*chan);
			chan->remove_user(*user);
			chan->remove_voice(*user);
			chan->remove_oper(*user);
		}
	}
	return "OK";
}

__WUR
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

__WUR
std::string
tlucanti::IRCParser::compose_names()
{
	user->assert_mode("reg+");

	if (chan_list.empty())
	{
		Database::channel_container_type all_channels = database.get_channels();
		Database::channel_container_type::iterator it = all_channels.begin();
		for (; it != all_channels.end(); ++it)
		{
			channel = it->second->get_name();
			std::string response = compose_names_single();
			if (not response.empty())
				user->send_message(response);
		}
		channel = "*";
	}
	else
	{
		if (chan_list.size() > 1)
			user->send_message(IRC::compose_message(nullptr, "NOTICE", *user, "you can use command only for one channel"));
		channel = *chan_list.begin();
		std::string response = compose_names_single();
		if (not response.empty())
			user->send_message(response);
	}
	user->send_message(IRC::RPL_ENDOFNAMES(*user, channel));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_names_single() const
{
	Channel *chan = database.get_channel(channel);
	char c = '=';

	user->assert_mode("reg+");
	if (chan == nullptr)
		return "";
	if (chan->has_mode("s+"))
		c = '@';
	if (c == '@' and (*chan)[*user] == nullptr)
		return "";
	return IRC::RPL_NAMREPLY(*user, c, *chan, chan->get_users(), *chan);
}

__WUR
std::string
tlucanti::IRCParser::compose_list()
{
	user->assert_mode("reg+");

	user->send_message(IRC::RPL_LISTSTART(*user));
	if (chan_list.empty())
	{
		Database::channel_container_type all_channels = database.get_channels();
		Database::channel_container_type::iterator it = all_channels.begin();
		for (; it != all_channels.end(); ++it)
		{
			channel = it->second->get_name();
			std::string response = compose_list_single();
			if (not response.empty())
				user->send_message(response);
		}
	}
	else
	{
		IRCParser::arg_list_type::iterator it = chan_list.begin();
		for (; it != chan_list.end(); ++it)
		{
			channel = *it;
			std::string response = compose_names_single();
			if (not response.empty())
				user->send_message(response);
		}
	}
	user->send_message(IRC::RPL_LISTEND(*user));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_list_single() const
{
	Channel *chan = database.get_channel(channel);

	user->assert_mode("reg+");
	if (chan == nullptr or chan->has_mode("s+"))
		return "";
	std::string _topic = chan->get_topic();
	if (_topic.empty())
		_topic = "[No topic]";
	return IRC::RPL_LIST(*user, *chan, chan->get_users().size(), _topic);
}

__WUR
std::string
tlucanti::IRCParser::compose_invite() const
{
	Channel *chan = database.get_channel(channel);
	User *tar = database[nickname];

	user->assert_mode("reg+");
	if (tar == nullptr)
		return IRC::ERR_NOSUCHNICK(*user, nickname, "user with nickname");
	if (chan == nullptr)
		return IRC::ERR_NOSUCHCHANNEL(*user, channel, "channel with name");
	if ((*chan)[*user] == nullptr)
		return IRC::ERR_NOTONCHANNEL(*user, *chan);
	if (chan->has_mode("i+") and not chan->is_oper(*user))
		return IRC::ERR_CHANOPRIVSNEEDED(*user, *chan);
	if ((*chan)[*tar] != nullptr)
		return IRC::ERR_USERONCHANNEL(*user, *tar, *chan);
	if (tar->has_mode("full+"))
		return IRC::ERR_TOOMANYCHANNELS(*user, *chan);
	if (chan->has_mode("full+") and not chan->is_oper(*user)) // operator can add beyond the limit
		return IRC::ERR_CHANNELISFULL(*user, *chan);
	// ok adding user
	user->send_message(IRC::RPL_INVITING(*user, *tar, *chan));
	tar->send_message(IRC::compose_message(user->compose(), "INVITE", *tar, channel));
	database.make_invite(*tar, *chan);
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_kick()
{
	Channel *chan = database.get_channel(channel);

	user->assert_mode("reg+");
	if (chan == nullptr)
		return IRC::ERR_NOSUCHCHANNEL(*user, channel, "channel with name");
	if (not (*chan)[*user])
		return IRC::ERR_NOTONCHANNEL(*user, *chan);
	if (not chan->is_oper(*user))
		return IRC::ERR_CHANOPRIVSNEEDED(*user, *chan);
	arg_list_type::const_iterator it = user_list.begin();
	for (; it != user_list.end(); ++it)
	{
		User *tar = dynamic_cast<User *>((*chan)[*it]);

		if (tar == nullptr)
		{
			user->send_message(IRC::ERR_USERNOTINCHANNEL(*user, *it, *chan));
			continue ;
		}
		tar->remove_channel(*chan);
		chan->remove_user(*tar);
		chan->remove_voice(*tar);
		chan->remove_oper(*tar);
		if (message.empty())
			message = "Your behavior is not conducive to the desired environment.";
		tar->send_message(IRC::compose_message(user->compose(), "KICK", chan->get_name() + ' ' + tar->get_name(), message));
	}
	return "";
}

// ------------------------ Server Queries and Commands ------------------------
__WUR
std::string
tlucanti::IRCParser::compose_motd() const
{
	user->assert_mode("reg+");
	user->send_message(IRC::RPL_MOTDSTART(*user));
#ifdef __CMAKE
	user->send_message(IRC::RPL_MOTD(*user, "../MOTD.txt"));
#else /* not __CMAKE */
	user->send_message(IRC::RPL_MOTD(*user, "MOTD.txt"));
#endif /* __CMAKE */
	user->send_message(IRC::RPL_ENDOFMOTD(*user));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_version() const
{
	user->assert_mode("reg+");
	user->send_message(IRC::RPL_VERSION(*user, tlucanti::server_version,
		tlucanti::server_address, ""));
	user->send_message(IRC::RPL_ISUPPORT(*user));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_admin() const
{
	user->assert_mode("reg+");
	user->send_message(IRC::RPL_ADMINME(*user, tlucanti::server_name));
	user->send_message(IRC::RPL_ADMINLOC1(*user, "server located in Moscow Russia"));
	user->send_message(IRC::RPL_ADMINLOC2(*user, "Server hosted by tlucanti"));
	user->send_message(IRC::RPL_ADMINEMAIL(*user, "Admin email: prototype3628800@yandex.eu"));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_time() const
{
	user->assert_mode("reg+");
	return IRC::RPL_TIME(*user);
}

__WUR
std::string
tlucanti::IRCParser::compose_help()
{
	std::string recipient = "*";
	if (user->has_mode("nick+"))
		recipient = user->get_name();
	const char *fname = "../README.md";
	{
		struct stat buffer = {};
		if (stat(fname, &buffer))
			return IRC::ERR_HELPNOTFOUND(recipient, "*");
	}
	std::transform(message.begin(), message.end(), message.begin(), toupper);
	user->send_message(IRC::RPL_HELPSTART(recipient, message, " >>> TLUCANTI IRC SERVER MANUAL <<< "));
	user->send_message(IRC::RPL_HELPTXT(recipient, message, fname));
	user->send_message(IRC::RPL_ENDOFHELP(recipient, message));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_info() const
{
	user->send_message(IRC::RPL_INFO(*user));
	user->send_message(IRC::RPL_ENDOFINFO(*user));
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_mode()
{
	user->assert_mode("reg+");
	std::string mode_string = mode;
	if (mode_string.empty())
	{
		has_suffix = 0;
		return compose_mode_single();
	}
	mode = "~0";
	char sign = '~';
	std::string::iterator it = mode_string.begin();
	arg_list_type::iterator arg_i = modes_list.begin();
	for (; it != mode_string.end(); ++it)
	{
		if (*it == '+' or *it == '-')
		{
			sign = *it;
			continue ;
		}
		else
		{
			mode.at(0) = sign;
			mode.at(1) = *it;
		}
		message = "";
		has_suffix = 1;
		if (mode == "+l" or mode == "+k" or mode.at(1) == 'v' or mode.at(1) == 'o' or mode == "+b" or mode == "+e")
		{
			if (arg_i != modes_list.end())
			{
				message = *arg_i;
				has_suffix = 2;
				++arg_i;
			}
			else
				has_suffix = 1;
		}
		std::string response = compose_mode_single();
		if (not response.empty())
			user->send_message(response);
	}

	std::string out_message;
	{
		std::stringstream ss;
		ss << mode_string;
		arg_list_type::iterator m_it = modes_list.begin();
		for (; m_it != modes_list.end(); ++m_it)
			ss << ' ' << *m_it;
		out_message = IRC::compose_message(user->compose(), "MODE", target, ss.str());
	}
	if (target.at(0) == '#' or target.at(0) == '&') // send new modes to the channel
		database.get_channel(target)->send_message(out_message);
	else
		user->send_message(out_message);
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_mode_single()
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
			else // cannot change mode
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
		if (mode == "i+" or mode == "i-" or mode == "k-" or mode == "m+" or mode == "m-" or mode == "s+" or mode == "s-" or mode == "t+" or mode == "t-" or mode == "n+" or mode == "n-")
		{
			if (has_suffix > 1)
				user->send_message(IRC::ERR_INVALIDMODEPARAM(*user, *chan, mode.at(0), truncate(message, 10), "extra tokens ignored"));
			chan->make_mode(mode);
		}
		else if (mode == "l+")
		{
			if (has_suffix == 1)
				return IRC::ERR_NEEDMOREPARAMS(*user, "MODE", "expected channel limit");
			unsigned int n;
			try {
				n = lexical_cast<int, 10>(message);
				if (n <= 0 or n > 500)
					return IRC::ERR_INVALIDMODEPARAM(*user, *chan, mode.at(0), message, "limit value should be in range [1:500]");
			} catch (tlucanti::bad_lexical_cast &) {
				return IRC::ERR_INVALIDMODEPARAM(*user, *chan, mode.at(0), message, "invalid limit value");
			}
			chan->make_limit(n);
		}
		else if (mode == "k+")
		{
			if (has_suffix == 1)
				return IRC::ERR_NEEDMOREPARAMS(*user, "MODE", "expected channel password");
			chan->make_pass(message);
			chan->make_mode("k+");
		}
		else if (mode == "e+")
		{
			if (has_suffix == 1)
				return IRC::ERR_NEEDMOREPARAMS(*user, "MODE", "expected username");
			User *pardon = database[message];
			if (pardon == nullptr)
				return IRC::ERR_NOSUCHNICK(*user, message, "user with nickname");
			chan->remove_ban(*pardon);

		}
		else if (mode == "v+" or mode == "o+" or mode == "v-" or mode == "o-" or mode == "b+")
		{
			if (has_suffix == 1)
				return IRC::ERR_NEEDMOREPARAMS(*user, "MODE", "expected username");
			User *recipient = dynamic_cast<User *>((*chan)[message]);
			if (recipient == nullptr)
				return IRC::ERR_USERNOTINCHANNEL(*user, message, *chan);
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
			else if (mode == "b+")
			{
				chan->add_ban(*recipient);
				recipient->remove_channel(*chan);
				chan->remove_user(*recipient);
				chan->remove_voice(*recipient);
				chan->remove_oper(*recipient);
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
	return "";

}

// ----------------------------- Sending Messages ------------------------------
__WUR
std::string
tlucanti::IRCParser::compose_msg(const char *type)
{
	user->assert_mode("reg+");
	arg_list_type::const_iterator it = user_list.begin();
	for (; it != user_list.end(); ++it)
	{
		target = *it;
		compose_msg_usr_single(type);
	}
	it = chan_list.begin();
	for (; it != chan_list.end(); ++it)
	{
		channel = *it;
		std::string response = compose_msg_chan_single(type);
		if (not response.empty())
			user->send_message(response);
	}
	return "OK";
}

__WUR
std::string
tlucanti::IRCParser::compose_privmsg()
{
	return compose_msg("PRIVMSG");
}
__WUR
std::string
tlucanti::IRCParser::compose_notice()
{
	return compose_msg("NOTICE");
}

__WUR
std::string
tlucanti::IRCParser::compose_msg_chan_single(const char *type) const
{
	Channel *chan = database.get_channel(channel);
	if (chan == nullptr) // channel not exist
		return IRC::ERR_NOSUCHCHANNEL(*user, channel, "channel with name");
	if (chan->is_banned(*user)) // user banned from channel
		return IRC::ERR_CANNOTSENDTOCHAN(*user, *chan, "you are banned from channel");
	if (chan->has_mode("n+") and (*chan)[*user] == nullptr) // no external messages
		return IRC::ERR_CANNOTSENDTOCHAN(*user, *chan, "you are have to be in this channel to send message");
	if (chan->has_mode("m+") and not chan->is_voice(*user)) // moderated channel
		return IRC::ERR_CANNOTSENDTOCHAN(*user, *chan, "you are have no permission to speak on this channel");
	// ok, sending message
	chan->send_message(IRC::RPL_AWAY(*user, *chan, type, message), *user);
	return "";
}

void
tlucanti::IRCParser::compose_msg_usr_single(const char *type) const
{
	User *cli = database[target];
	if (cli == nullptr)
		user->send_message(IRC::ERR_NOSUCHNICK(*user, target, "user with nickname"));
	else
		cli->send_message(IRC::RPL_AWAY(*user, *cli, type, message));
}

// ---------------------------- User-Based Queries -----------------------------
__WUR
std::string
tlucanti::IRCParser::compose_who() const
{
//	{
//		User *tar = database[target];
//		if (tar == nullptr)
//			std::cout << "target not found\n";
//
//		int peer_sock = tar->get_sock().get_sock();
//		char ip_str[INET_ADDRSTRLEN];
//		struct sockaddr_storage peer_addr {};
//		socklen_t len = sizeof peer_addr;
//		getpeername(peer_sock, reinterpret_cast<sockaddr *>(&peer_addr), &len);
//
//		if (::bind(user->get_sock().get_sock(), reinterpret_cast<const sockaddr *>(&peer_addr), sizeof(peer_addr)))
//			perror("");
//		return "";
//	}
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
		{
			if ((*it)->has_mode("i+") and (*chan)[*user] == nullptr)
				continue ;
			user->send_message(IRC::RPL_WHOREPLY(*user, dynamic_cast<const User &>(**it), *chan));
		}
	}
	else // user who
	{
		ITarget *tar = database[target];
		if (tar == nullptr or tar->has_mode("i+"))
			return IRC::ERR_NOSUCHNICK(*user, target, "user with nickname");
		user->send_message(IRC::RPL_WHOREPLY(*user, dynamic_cast<const User &>(*tar), '*'));
	}
	return IRC::RPL_ENDOFWHO(*user, target);
}

__WUR
std::string
tlucanti::IRCParser::compose_ison()
{
	user->assert_mode("reg+");

	arg_list_type::iterator it = user_list.begin();
	for(; it != user_list.end(); ++it)
	{
		User *tar = database[*it];
		if (tar == nullptr)
			it = --user_list.erase(it);
	}
	return IRC::RPL_ISON(*user, user_list);
}

// ----------------------------- Operator Messages -----------------------------
__WUR
std::string
tlucanti::IRCParser::compose_kill() const
{
	User *tar = database[nickname];

	user->assert_mode("reg+");
	if (user->has_mode("o-"))
		return IRC::ERR_NOPRIVILEGES(*user);
	if (tar == nullptr)
		return IRC::ERR_NOSUCHNICK(*user, nickname, "user with nickname");
//	arg_list_type::const_iterator it = user_list.begin();
	std::string _response;
	{
		std::stringstream ss;
		ss << "Killed by " << user->get_name() << " (" << message << ')';
		_response = ss.str();
	}
	tar->send_to_channels(IRC::compose_message(user->compose(), "QUIT", *tar, _response));
	tar->send_message(IRC::ERROR(_response));
	database.remove_client(*tar);
	return "";
}

__WUR
std::string
tlucanti::IRCParser::compose_restart() const
{
	user->assert_mode("reg+");
	if (user->has_mode("o-"))
		return IRC::ERR_NOPRIVILEGES(*user);
	database.send_to_all(IRC::compose_message(nullptr, "NOTICE", '*',
		"WARNING! SERVER RESTARTING"));
	server_int = 'r';
	database.collapse();
	throw Exception("server", "called restart command");
}

__WUR
std::string
tlucanti::IRCParser::compose_squit() const
{
	user->assert_mode("reg+");
	if (user->has_mode("o-"))
		return IRC::ERR_NOPRIVILEGES(*user);
	database.send_to_all(IRC::compose_message(nullptr, "NOTICE", '*',
		"WARNING! SERVER SHUTTING DOWN"));
	server_int = 'q';
	database.collapse();
	throw Exception("server", "called squit command");
}
