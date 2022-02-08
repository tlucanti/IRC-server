/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCrpl.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 11:05:01 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/08 22:09:55 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCRPL_HPP
# define IRCRPL_HPP

#include <fstream>

#include "parser_utils.hpp"

namespace tlucanti
{
	extern std::string server_name;
	extern std::string server_password;
	extern std::string server_address;
	extern std::string server_begining;
	extern std::string server_version;
	extern unsigned short server_port;
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

	template <typename nick_T>
	inline std::string RPL_WELCOME(const nick_T &nickname)
	/*
		:`SERVER` 001 `NICK` :Welcome, `NICK`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_WELCOME <<
			' ' << nickname << " :Welcome to the Tlucanti Internet Relay Chat "
			"Network, " << nickname << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_YOURHOST(const nick_T &nickname)
	/*
		:`SERVER` 002 `NICK` :Your host is [`HOST`/`PORT`] version `VERSION`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_YOURHOST <<
			' ' << nickname << " :Your host is " << tlucanti::server_name <<
			'[' << tlucanti::server_address << '/' << tlucanti::server_port <<
			"], running version " << tlucanti::server_version << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_CREATED(const nick_T &nickname)
	/*
		:`SERVER` 003 `NICK` :This server was created `DATE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_CREATED <<
			' ' << nickname << " :This server was created " <<
			tlucanti::server_begining << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_MYINFO(const nick_T &nickname, const char *user_modes, const char *channel_modes)
	/*
		:`SERVER` 004 `NICK` `SERVER` `VERSION` `USERMODES` `CHANNELMODES`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_MYINFO <<
			' ' << nickname << ' ' << tlucanti::server_name << ' ' <<
			tlucanti::server_version << ' ' << user_modes << ' ' <<
			channel_modes << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_ISUPPORT(const nick_T &nickname)
	/*
		:`SERVER` 005 `NICK` `FEATURES` :are supported by this server
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ISUPPORT <<
			' ' << nickname << " SAFELIST CHANTYPES=&# CHANLIMIT=&#:32 MODES= "
			"NETWORK=Tlucanti :are supported by this server" << IRC::endl;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ISUPPORT <<
		   ' ' << nickname << " CASEMAPPING=rfc1459 NICKLEN=20 MAXNICKLEN=20 "
		   "CHANNELLEN=50 TOPICLEN=300 :are supported by this server" <<
		   IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_LUSERCLIENT(const nick_T &nickname, int users,
		int invisible, int servers)
	/*
		:`SERVER` 251 `NICK` :`USERS` users `INV` invisible `SERVERS` servers
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::RPL_LUSERCLIENT << ' ' << nickname << " :There are " <<
			users << " users and " << invisible << " invisible on " <<
			servers << " servers" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_LUSEROP(const nick_T &nickname, int operators)
	/*
		:`SERVER` 252 `NICK` `OPS` :operator(s) online
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_LUSEROP <<
			' ' << nickname << ' ' << operators << " :operator(s) online" <<
			IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_LUSERCHANNELS(const nick_T &nickname, int channels)
	/*
		:`SERVER` 254 `NICK` `CHANNELS` :channels formed
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::RPL_LUSERCHANNELS << ' ' << nickname << ' ' << channels <<
			" :channels formed" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_LUSERME(const nick_T &nickname, int clients,
		int servers)
	/*
		:`SERVER` 255 `NICK` :I have `CLIENTS` clients and `SERVERS` servers
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_LUSERME <<
			' ' << nickname << " :I have " << clients << " clients and " <<
			servers << " servers" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_LOCALUSERS(const nick_T &nickname, int users,
		int maxusers)
	/*
		:`SERVER` 265 `NICK` `USERS` `MAX` :local `USERS` users, max `MAX`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_LOCALUSERS <<
			' ' << nickname << ' ' << users << ' ' << maxusers <<
			" :Current local users " << users << ", max " << maxusers <<
			IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_GLOBALUSERS(const nick_T &nickname, int glbl_users,
		int glbl_maxusers)
	/*
		:`SERVER` 266 `NICK` `USERS` `MAX` :global `USERS` users, max `MAX`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::RPL_GLOBALUSERS << ' ' << nickname << ' ' << glbl_users <<
			' ' << glbl_maxusers << " :Current global users " << glbl_users <<
			", max " << glbl_maxusers << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_MOTD(const nick_T &nickname, const char *fname)
	/*
		:`SERVER` 372 `NICK` :- `LINE OF MOTD`
	*/
	{
		std::string start;
		{
			std::stringstream ss;
			ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_MOTD <<
			   ' ' << nickname << " :- ";
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

	template <typename nick_T>
	inline std::string RPL_MOTDSTART(const nick_T &nickname)
	/*
		:`SERVER` 375 `NICK` :- `SERVER` Message of the day -
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_MOTDSTART <<
			' ' << nickname << " :- `SERVER` Message of the day -" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string RPL_ENDOFMOTD(const nick_T &nickname)
	/*
		:`SERVER` 376 `NICK` :End of /MOTD command.
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::RPL_ENDOFMOTD <<
			' ' << nickname << " :End of /MOTD command." << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_NOSUCHNICK(const nick_T &nickname,
		const std::string &recepient, const std::string &what)
	/*
		:`SERVER` 401 `NICK` `RECEPIENT` :`WHAT` with `RECEPIENT` does not exist
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NOSUCHNICK <<
			' ' << nickname << ' ' << recepient << " :" << what << " `" <<
			recepient << "` does not exist" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T, typename chan_T>
	inline std::string ERR_NOSUCHCHANNEL(const nick_T &nickname,
		const chan_T &channel, const std::string &message)
	/*
		:`SERCER` 403 `NICK` `CHANNEL` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NOSUCHCHANNEL <<
			' ' << nickname << ' ' << channel << " :" << message << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_NORECIPIENT(const nick_T &nickname,
		const std::string &message)
	/*
		:`SERVER` 411 `NICK` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NORECIPIENT <<
			' ' << nickname << " :" << message << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_NOTEXTTOSEND(const nick_T &nickname)
	/*
		:`SERVER` 412 `NICK` :Expected message
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NOTEXTTOSEND <<
			' ' << nickname << " :Expected message" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_UNKNOWNCOMMAND(const nick_T &nickname,
		const std::string &command)
	/*
		:`SERVER` 421 `NICK` `COMMAND` :Command `COMMAND` is unknown
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_UNKNOWNCOMMAND <<
			' ' << nickname << ' ' << command << " :Command is unknown" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_NONICKNAMEGIVEN(const nick_T &nickname)
	/*
		:`SERVER` 431 `NICK` :Expected nickname
	*/
	{
		std::stringstream ss;
		ss << ':' + tlucanti::server_name << ' ' << IRCcodes::ERR_NONICKNAMEGIVEN <<
			' ' << nickname << " :Expected nickname" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T, typename new_nick_T>
	inline std::string ERR_NICKNAMEINUSE(const nick_T &nickname,
		const new_nick_T &new_nick)
	/*
		:`SERVER` 433 `NICK` `NEWNICK` :Nickname `NEWNICK` is already in use.
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_NICKNAMEINUSE <<
			' ' << nickname << ' ' << new_nick << " :Nickname " << new_nick <<
			" is already in use." << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_NOTREGISTERED(const nick_T &nickname)
	/*
		:`SERVER` 451 `NICK` :You need to register
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_NOTREGISTERED << ' ' << nickname << " :You need to register to use this command" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_NEEDMOREPARAMS(const nick_T &nickname,
		const std::string &command, const std::string &message)
	/*
		:`SERVER` 461 `NICK` `COMMAND` :`MESSAGE`
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_NEEDMOREPARAMS << ' ' << nickname << ' ' << command <<
			" :" << message << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_ALREADYREGISTERED(const nick_T &nickname)
	/*
		:`SERVER` 462 `NICK` :You are already registered
	*/
	{
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' << IRCcodes::ERR_ALREADYREGISTERED << ' ' << nickname << " :You are already registered" << IRC::endl;
		return ss.str();
	}

	template <typename nick_T>
	inline std::string ERR_PASSWDMISMATCH(const nick_T &nickname)
	{
		// :SERVER 464 `NICK` :Password incorrect
		std::stringstream ss;
		ss << ':' << tlucanti::server_name << ' ' <<
			IRCcodes::ERR_PASSWDMISMATCH << ' ' << nickname << " :Password incorrect" << IRC::endl;
		return ss.str();
	}
}

#endif /* IRCRPL_HPP */
