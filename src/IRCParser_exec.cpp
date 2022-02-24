/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_exec.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:55:16 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 13:59:04 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

namespace tlucanti {
	extern tlucanti::Database database;
}

std::string
tlucanti::IRCParser::exec(const Socket &client)
{
	init();
	if (has_preffix)
	{
		user = database[prefix];
		if (user == nullptr)
			return IRC::ERR_NOSUCHNICK(database[prefix], prefix, "user with nickname");
	}
	else
		user = database[client];

	if (command.empty())
		return "";
	parse();
	if ((command == "PRIVMSG" or command == "NOTICE") and dcc)
	{
		std::cout << "User ip: " << user->get_sock().get_address() << ':' << user->get_sock().get_port() << std::endl;
//		return compose_dcc(message);
	}
//	else if (dcc)
//		throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "message can contain only printable characters"));


// ---------------------------- Connection Messages ----------------------------
	if (command == "CAP")
		return compose_cap();
	if (command == "PASS")
		return compose_pass();
	else if (command == "NICK")
		return compose_nick();
	else if (command == "USER")
		return compose_user();
	else if (command == "PING")
		return compose_ping();
	else if (command == "PONG")
		return compose_pong();
	else if (command == "OPER")
		return compose_oper();
	else if (command == "QUIT")
		return compose_quit();
// ---------------------------- Channel Operations -----------------------------
	else if (command == "JOIN")
		return compose_join();
	else if (command == "PART")
		return compose_part();
	else if (command == "TOPIC")
		return compose_topic();
	else if (command == "NAMES")
		return compose_names();
	else if (command == "LIST")
		return compose_list();
	else if (command == "INVITE")
		return compose_invite();
	else if (command == "KICK")
		return compose_kick();
// ------------------------ Server Queries and Commands ------------------------
	else if (command == "MOTD")
		return compose_motd();
	else if (command == "VERSION")
		return compose_version();
	else if (command == "ADMIN")
		return compose_admin();
	else if (command == "TIME")
		return compose_time();
	else if (command == "HELP")
		return compose_help();
	else if (command == "INFO")
		return compose_info();
	else if (command == "MODE")
		return compose_mode();
// ----------------------------- Sending Messages ------------------------------
	else if (command == "PRIVMSG")
		return compose_privmsg();
	else if (command == "NOTICE")
		return compose_notice();
// ---------------------------- User-Based Queries -----------------------------
	else if (command == "WHO")
		return compose_who();
	else if (command == "ISON")
		return compose_ison();
// ----------------------------- Operator Messages -----------------------------
	else if (command == "KILL")
		return compose_kill();
	else if (command == "RESTART")
		return compose_restart();
	else if (command == "SQUIT")
		return compose_squit();

	else
		ABORT("invalid command", command);
	ABORT("implement command", command);
}
