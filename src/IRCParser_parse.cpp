/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser_parse.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:55:13 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 14:56:03 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

void
tlucanti::IRCParser::parse()
{
	// reformat this function and remove if/else
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

// ---------------------------- Connection Messages ----------------------------
	if (command == "CAP")
	{}
	else if (command == "PASS")
	{
		check_format(line, "cmd", "pass");
		password = line.at(1);
	}
	else if (command == "NICK")
	{
		check_format(line, "[:nick]", "cmd", "nick");
		nickname = line.at(1);
	}
	else if (command == "USER")
	{
		check_format(line, "[:nick]", "cmd", "nick", "any", "any", ":msg");
		nickname = line.at(1);
		realname = line.at(4);
	}
	else if (command == "PING")
	{
		check_format(line, "[:nick]", "cmd", ":pass");
		message = line.at(1);
	}
	else if (command == "PONG")
	{
		check_format(line, "[:nick]", "cmd", "[:pass]");
		if (has_suffix)
			message = line.at(1);
	}
	else if (command == "OPER")
	{
		check_format(line, "[:nick]", "cmd", "str", "pass");
		nickname = line.at(1);
		password = line.at(2);
	}
	else if (command == "QUIT")
	{
		check_format(line, "cmd", "[:msg]");
		if (has_suffix)
			message = line.at(1);
		else
			message = "Quit from server";
	}
	else if (command == "ERROR")
	{
		check_format(line, "cmd", ":msg");
		message = line.at(1);
	}
// ---------------------------- Channel Operations -----------------------------
	else if (command == "JOIN")
		check_format(line, "[:nick]", "cmd", "chan_list", "[pass_list]");
	else if (command == "PART")
	{
		check_format(line, "[:nick]", "cmd", "chan_list", "[:msg]");
		if (has_suffix)
			message = line.at(2);
		else
			message = "Quit from channel";
	}
	else if (command == "TOPIC")
	{
		check_format(line, "[:nick]", "cmd", "chan", "[:msg]");
		channel = line.at(1);
		if (has_suffix)
			message = line.at(2);
		else
			message = "";
	}
	else if (command == "NAMES")
		check_format(line, "[:nick]", "cmd", "[chan_list]");
	else if (command == "LIST")
		check_format(line, "[:nick]", "cmd", "[chan_list]");
	else if (command == "INVITE")
	{
		check_format(line, "[:nick]", "cmd", "nick", "chan");
		nickname = line.at(1);
		channel = line.at(2);
	}
	else if (command == "KICK")
	{
		check_format(line, "[:nick]", "cmd", "chan", "user_list", "[:msg]");
		channel = line.at(1);
		nickname = line.at(2);
		if (has_suffix)
			message = line.at(3);
		else
			message = "Kicked out from server";
	}
// ------------------------ Server Queries and Commands ------------------------
	else if (command == "MOTD")
		check_format(line, "[:nick]", "cmd");
	else if (command == "VERSION")
		check_format(line, "[:nick]", "cmd");
	else if (command == "ADMIN")
		check_format(line, "[:nick]", "cmd");
	else if (command == "TIME")
		check_format(line, "[:nick]", "cmd");
	else if (command == "HELP")
	{
		check_format(line, "[:nick]", "cmd", "[str]");
		if (has_suffix)
			message = line.at(1);
		else
			message = "";
	}
	else if (command == "INFO")
		check_format(line, "[:nick]", "cmd");
	else if (command == "MODE")
	{
		check_format(line, "[:nick]", "cmd", "target", "[any]", "[mode_list]");
		target = line.at(1);
		if (has_suffix >= 1)
			mode = line.at(2);
		else
			mode = "";
	}
// ----------------------------- Sending Messages ------------------------------
	else if (command == "PRIVMSG")
	{
		check_format(line, "[:nick]", "cmd", "receiver_list", ":msg");
		message = line.at(2);
	}
	else if (command == "NOTICE")
	{
		check_format(line, "[:nick]", "cmd", "receiver_list", ":msg");
		message = line.at(2);
	}
// ---------------------------- User-Based Queries -----------------------------
	else if (command == "WHO")
	{
		check_format(line, "[:nick]", "cmd", "target");
		target = line.at(1);
	}
	else if (command == "ISON")
		check_format(line, "[:nick]", "cmd", "user_sequence");
// ----------------------------- Operator Messages -----------------------------
	else if (command == "KILL")
	{
		check_format(line, "cmd", "nick", ":msg");
		nickname = line.at(1);
		message = line.at(2);
	}
	else if (command == "RESTART")
		check_format(line, "cmd");
	else if (command == "SQUIT")
		check_format(line, "cmd");
	else
		throw IRCParserException(IRC::ERR_UNKNOWNCOMMAND(*user, command));
}
