/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCrpl.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 11:05:01 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/10 20:55:10 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCRPL_HPP
# define IRCRPL_HPP

#include <fstream>

#include "parser_utils.hpp"

namespace tlucanti
{
	extern const char *server_name;
	extern const char *server_address;
	extern const char *server_version;
	extern const char *server_oper_login;
	extern const char *server_oper_password;

	extern std::string server_password;
	extern std::string server_begining;
	extern unsigned short server_port;

	extern const unsigned int channel_max_users;
	extern const unsigned int chanel_max_name_len;
	extern const unsigned int channel_max_topic_len;
	extern const unsigned int user_max_channels;
	extern const unsigned int user_max_nick_len;
}

namespace tlucanti::IRC
{
	template <typename from_T, typename to_T>
	inline std::string compose_message(const from_T &from, const char *command,
		const to_T &to, const std::string &message, bool colon=true)
	/*
		:`FROM` `COMMAND` `TO` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':';
		if (tlucanti::isinstance<std::nullptr_t>(from))
			ss << tlucanti::server_name;
		else
			ss << from;
		ss << ' ' << command << ' ' << to << ' ';
		if (colon)
			ss << ':';
		ss << message << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_WELCOME(const target_T &target)
	/*
		:`SERVER` 001 `NICK` :Welcome, `NICK`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_WELCOME <<
			' ' << target << " :Welcome to the Tlucanti Internet Relay Chat "
			"Network, " << target << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_YOURHOST(const target_T &target)
	/*
		:`SERVER` 002 `NICK` :Your host is [`HOST`/`PORT`] version `VERSION`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_YOURHOST <<
			' ' << target << " :Your host is " << tlucanti::server_name <<
			'[' << tlucanti::server_address << '/' << tlucanti::server_port <<
			"], running version " << tlucanti::server_version << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_CREATED(const target_T &target)
	/*
		:`SERVER` 003 `NICK` :This server was created `DATE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_CREATED <<
			' ' << target << " :This server was created " <<
			tlucanti::server_begining << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_MYINFO(const target_T &target, const char *user_modes, const char *channel_modes)
	/*
		:`SERVER` 004 `NICK` `SERVER` `VERSION` `USERMODES` `CHANNELMODES`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_MYINFO <<
			' ' << target << ' ' << tlucanti::server_name << ' ' <<
			tlucanti::server_version << ' ' << user_modes << ' ' <<
			channel_modes << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_ISUPPORT(const target_T &target)
	/*
		:`SERVER` 005 `NICK` `FEATURES` :are supported by this server
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ISUPPORT <<
			' ' << target << " SAFELIST CHANTYPES=&# CHANLIMIT=&#:32 MODES=1 "
			"NETWORK=Tlucanti :are supported by this server" << IRC::endl;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ISUPPORT <<
		   ' ' << target << " CASEMAPPING=rfc1459 NICKLEN=20 MAXNICKLEN=20 "
		   "CHANNELLEN=50 TOPICLEN=300 :are supported by this server" <<
		   IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_UMODEIS(const target_T &target,
		const std::string &modes)
	/*
		:`SERVER` 221 `NICK` `MODES`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_UMODEIS <<
			' ' << target << ' ' << modes << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_LUSERCLIENT(const target_T &target, int users,
		int invisible, int servers)
	/*
		:`SERVER` 251 `NICK` :`USERS` users `INV` invisible `SERVERS` servers
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::RPL_LUSERCLIENT << ' ' << target << " :There are " <<
			users << " users and " << invisible << " invisible on " <<
			servers << " servers" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_LUSEROP(const target_T &target, int operators)
	/*
		:`SERVER` 252 `NICK` `OPS` :operator(s) online
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_LUSEROP <<
			' ' << target << ' ' << operators << " :operator(s) online" <<
			IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_LUSERCHANNELS(const target_T &target, int channels)
	/*
		:`SERVER` 254 `NICK` `CHANNELS` :channels formed
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::RPL_LUSERCHANNELS << ' ' << target << ' ' << channels <<
			" :channels formed" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_LUSERME(const target_T &target, int clients,
		int servers)
	/*
		:`SERVER` 255 `NICK` :I have `CLIENTS` clients and `SERVERS` servers
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_LUSERME <<
			' ' << target << " :I have " << clients << " clients and " <<
			servers << " servers" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_LOCALUSERS(const target_T &target, int users,
		int maxusers)
	/*
		:`SERVER` 265 `NICK` `USERS` `MAX` :local `USERS` users, max `MAX`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_LOCALUSERS <<
			' ' << target << ' ' << users << ' ' << maxusers <<
			" :Current local users " << users << ", max " << maxusers <<
			IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_GLOBALUSERS(const target_T &target, int glbl_users,
		int glbl_maxusers)
	/*
		:`SERVER` 266 `NICK` `USERS` `MAX` :global `USERS` users, max `MAX`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::RPL_GLOBALUSERS << ' ' << target << ' ' << glbl_users <<
			' ' << glbl_maxusers << " :Current global users " << glbl_users <<
			", max " << glbl_maxusers << IRC::endl;
		return ss.str();
	}

	template <typename target_T, typename to_T>
	inline std::string RPL_AWAY(const target_T &target, const to_T &recepient,
		const std::string &message)
	/*
		(301) :`NICK(full)` PRIVMSG `RECEPIENT` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << target.compose() << " PRIVMSG " <<
			recepient << " :" << message << IRC::endl;
		return ss.str();
	}

	template <typename target_T, typename chan_T, typename list_T>
	inline std::string RPL_NAMREPLY(const target_T &target, char symbol,
		const chan_T &channel, const list_T &user_list)
	/*
		:`SERVER` 353 `target` `SYM` `CHANNEL` :`USER1` `USER2` ...
	*/
	{
		std::string start;
		{
			std::stringstream ss;
			ss << ':' << tlucanti::server_name << ' ' <<
				IRCcodes::RPL_NAMREPLY << ' ' << target << ' ' << symbol <<
				' ' << channel << " :";
			start = ss.str();
		}
		std::stringstream ss;
		int dec = 0;
		for (typename list_T::const_iterator it=user_list.begin(); it != user_list.end(); ++it)
		{
			if (dec == 10)
			{
				ss << IRC::endl << start;
				dec = 0;
			}
			ss << ' ' << (**it);
			++dec;
		}
		return ss.str();
	}

	template <typename target_T, typename chan_T>
	inline std::string RPL_ENDOFNAMES(const target_T &target,
		const chan_T &channel)
	/*
		:`SERVER` 366 `target` `CHANNEL` :End of /NAMES list
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ENDOFNAMES <<
			target << ' ' << channel << " :End of /NAMES list" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_MOTD(const target_T &target, const char *fname)
	/*
		:`SERVER` 372 `NICK` :- `LINE OF MOTD`
	*/
	{
		std::string start;
		{
			std::stringstream ss;
			ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_MOTD <<
			   ' ' << target << " :- ";
			start = ss.str();
		}
		std::stringstream ss;
		std::ifstream fin(fname);
		while (fin)
		{
			std::string line;
			std::getline(fin, line);
			ss << start << line << IRC::endl;
		}
		fin.close();
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_MOTDSTART(const target_T &target)
	/*
		:`SERVER` 375 `NICK` :- `SERVER` Message of the day -
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_MOTDSTART <<
			' ' << target << " :- `SERVER` Message of the day -" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_ENDOFMOTD(const target_T &target)
	/*
		:`SERVER` 376 `NICK` :End of /MOTD command.
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ENDOFMOTD <<
			' ' << target << " :End of /MOTD command." << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string RPL_YOUREOPER(const target_T &target,
		const std::string &message)
	/*
		:`SERVER` 381 `NICK` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_YOUREOPER <<
			' ' << target << " :" << message << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_NOSUCHNICK(const target_T &target,
		const std::string &recepient, const std::string &what)
	/*
		:`SERVER` 401 `NICK` `RECEPIENT` :`WHAT` with `RECEPIENT` does not exist
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NOSUCHNICK <<
			' ' << target << ' ' << recepient << " :" << what << " `" <<
			recepient << "` does not exist" << IRC::endl;
		return ss.str();
	}

	template <typename target_T, typename chan_T>
	inline std::string ERR_NOSUCHCHANNEL(const target_T &target,
		const chan_T &channel, const std::string &message)
	/*
		:`SERVER` 403 `NICK` `CHANNEL` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NOSUCHCHANNEL <<
			' ' << target << ' ' << channel << " :" << message << IRC::endl;
		return ss.str();
	}

	template <typename target_T, typename chan_T>
	inline std::string ERR_TOOMANYCHANNELS(const target_T &target,
		const chan_T &channel)
	/*
		:`SERVER` 405 `NICK` `CHANNEL` :You have joined too many channels
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_TOOMANYCHANNELS << ' ' << target << ' ' <<
			channel << " :You have joined too many channels" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_NORECIPIENT(const target_T &target,
		const std::string &message)
	/*
		:`SERVER` 411 `NICK` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NORECIPIENT <<
			' ' << target << " :" << message << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_NOTEXTTOSEND(const target_T &target)
	/*
		:`SERVER` 412 `NICK` :Expected message
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NOTEXTTOSEND <<
			' ' << target << " :Expected message" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_UNKNOWNCOMMAND(const target_T &target,
		const std::string &command)
	/*
		:`SERVER` 421 `NICK` `COMMAND` :Command `COMMAND` is unknown
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_UNKNOWNCOMMAND <<
			' ' << target << ' ' << command << " :Command is unknown" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_NONICKNAMEGIVEN  (const target_T &target)
	/*
		:`SERVER` 431 `NICK` :Expected target
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NONICKNAMEGIVEN  <<
			' ' << target << " :Expected target" << IRC::endl;
		return ss.str();
	}

	template <typename target_T, typename new_target_T>
	inline std::string ERR_NICKNAMEINUSE(const target_T &target,
		const new_target_T &new_nick)
	/*
		:`SERVER` 433 `NICK` `NEWNICK` :target `NEWNICK` is already in use.
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NICKNAMEINUSE <<
			' ' << target << ' ' << new_nick << " :target " << new_nick <<
			" is already in use." << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_NOTREGISTERED(const target_T &target)
	/*
		:`SERVER` 451 `NICK` :You need to register
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_NOTREGISTERED << ' ' << target << " :You need to register to use this command" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_NEEDMOREPARAMS(const target_T &target,
		const std::string &command, const std::string &message)
	/*
		:`SERVER` 461 `NICK` `COMMAND` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_NEEDMOREPARAMS << ' ' << target << ' ' << command <<
			" :" << message << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_ALREADYREGISTERED(const target_T &target)
	/*
		:`SERVER` 462 `NICK` :You are already registered
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_ALREADYREGISTERED << ' ' << target << " :You are already registered" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_PASSWDMISMATCH(const target_T &target)
	/*
		:`SERVER` 464 `NICK` :Password incorrect
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_PASSWDMISMATCH << ' ' << target << " :Password incorrect" << IRC::endl;
		return ss.str();
	}

	template <typename target_T, typename chan_T>
	inline std::string ERR_CHANNELISFULL(const target_T &target,
		const chan_T &channel)
	/*
		:`SERVER` 464 `NICK` `CHANNEL` :Channel is full
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_CHANNELISFULL << target << ' ' << channel <<
			" :Channel is full" << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_UMODEUNKNOWNFLAG(const target_T &target,
		const std::string &mode)
	/*
		:`SERVER` 501 `NICK` :Unknown MODE flag `MODE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_UMODEUNKNOWNFLAG << ' ' << target <<
			" :Unknown MODE flag " << mode << IRC::endl;
		return ss.str();
	}

	template <typename target_T>
	inline std::string ERR_USERSDONTMATCH(const target_T &target)
	/*
		:`SERVER` 502 `NICK` :Can not change mode for other users
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_USERSDONTMATCH << ' ' << target <<
			" :Can not change mode for other users" << IRC::endl;
		return ss.str();
	}
}

#endif /* IRCRPL_HPP */
