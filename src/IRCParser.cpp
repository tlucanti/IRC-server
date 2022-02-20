/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:35:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/20 22:49:19 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

const char *tlucanti::IRCParser::PRINTABLE = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&'()*+-./:;<=>?@[\]^_`{|}~)";
const char *tlucanti::IRCParser::PRINTABLESPACE = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&'()*+-./:;<=>?@[\]^_`{|}~ )";
const char *tlucanti::IRCParser::ALNUM = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *tlucanti::IRCParser::NICKNAME = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-[]\`^{}*)";

namespace tlucanti {
	extern Database database;
}

tlucanti::IRCParser::IRCParser(const std::string &raw_command) :
		prefix("UNINITIALIZED_PREFFIX"),
		command("UNINITIALIZED_COMMAND"),
		nickname("UNINITIALIZED_NICKNAME"),
		password("UNINITIALIZED_PASSWORD"),
		realname("UNINITIALIZED_REALNAME"),
		message("UNINITIALIZED_MESSAGE"),
		channel("UNINITIALIZED_CHANNEL"),
		mode("UNINITIALIZED_MODESTRING"),
		target("UNINITIALIZED_TARGET"),
		_raw_command(raw_command),
		has_suffix(false),
		has_preffix(false),
		user(nullptr),
		dcc(false) {}

void
tlucanti::IRCParser::init()
{
	_raw_command = replace(_raw_command, "\r", "");
	_raw_command = replace(_raw_command, "\n", "");
	_raw_command = strip(_raw_command);
	if (_raw_command.empty())
		throw IRCParserException("");
	if (_raw_command.at(0) == ':')
		has_preffix = true;
	std::string _command_part, _suffix_part;
	std::size_t _suffix_start = std::min(_raw_command.size(), _raw_command.find(':', 1));
	_command_part = _raw_command.substr(0, _suffix_start);
	_suffix_part = _raw_command.substr(_suffix_start);
	_command_part = squeeze(_command_part); // multiple space -> single space
	_command_part = replace(_command_part, ", ", ","); // , ', ' -> ','
	if (not _suffix_part.empty())
		_raw_command = _command_part + ' ' + _suffix_part;
	else
		_raw_command = _command_part;
	split_string(_raw_command, line);
	has_suffix = false;

	if (line.empty() or (line.size() == 1 and has_preffix))
		throw IRCParserException("");

	arg_list_type::iterator it = line.begin();
	if (has_preffix)
		prefix = *(it++);
	command = *it; // iterator will valid because line.size() >= 1 (or >= 2 with prefix)
	while (++it != line.end())
		arguments.push_back(*it);
}


void
tlucanti::IRCParser::parse()
{
	typedef const char *fmt_t[];

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
		int add = 0;
		if (has_preffix)
			++add;
		nickname = line.at(1 + add);
	}
	else if (command == "USER")
	{
		check_format(line, "[:nick]", "cmd", "nick", "nick", "nick", ":msg");
		int add = 0;
		if (has_preffix)
			++add;
		nickname = line.at(1 + add);
		realname = line.at(4 + add);
	}
	else if (command == "PING")
	{
		check_format(line, "[:nick]", "cmd", ":pass");
		int add = 0;
		if (has_preffix)
			++add;
		message = line.at(1 + add);
	}
	else if (command == "PONG")
	{
		check_format(line, "[:nick]", "cmd", "pass");
		int add = 0;
		if (has_preffix)
			++add;
		message = line.at(1 + add);
	}
	else if (command == "OPER")
	{
		check_format(line, "[:nick]", "cmd", "nick", "pass");
		int add = 0;
		if (has_preffix)
			++add;
		nickname = line.at(1 + add);
		password = line.at(2 + add);
	}
	else if (command == "QUIT")
	{
		check_format(line, "cmd", "[:msg]");
		if (has_suffix)
			message = line.at(1);
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
		int add = 0;
		if (has_preffix)
			++add;
		if (has_suffix)
			message = line.at(2 + add);
	}
	else if (command == "TOPIC")
	{
		check_format(line, "[:nick]", "cmd", "chan", "[:msg]");
		int add = 0;
		if (has_preffix)
			++add;
		channel = line.at(1 + add);
		if (has_suffix)
			message = line.at(2 + add);
	}
	else if (command == "NAMES")
		check_format(line, "[:nick]", "cmd", "[chan_list]");
	else if (command == "LIST")
		check_format(line, "[:nick]", "cmd", "[chan_list]");
	else if (command == "INVITE")
	{
		check_format(line, "[:nick]", "cmd", "nick", "chan");
		int add = 0;
		if (has_preffix)
			++add;
		nickname = line.at(1 + add);
		channel = line.at(2 + add);
	}
	else if (command == "KICK")
	{
		check_format(line, "[:nick]", "cmd", "chan", "user_list", "[:msg]");
		int add = 0;
		if (has_preffix)
			++add;
		channel = line.at(1 + add);
		nickname = line.at(2 + add);
		if (has_suffix)
			message = line.at(3 + add);
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
		check_format(line, "[:nick]", "cmd", "str");
		int add = 0;
		if (has_preffix)
			++add;
		message = line.at(1 + add);
	}
	else if (command == "INFO")
		check_format(line, "[:nick]", "cmd");
	else if (command == "MODE")
	{
		check_format(line, "[:nick]", "cmd", "target", "[str]", "[mode_list]");
		int add = 0;
		if (has_preffix)
			++add;
		target = line.at(1 + add);
		if (has_suffix >= 1)
			mode = line.at(2 + add);
		else
			mode = "";
	}
// ----------------------------- Sending Messages ------------------------------
	else if (command == "PRIVMSG")
	{
		check_format(line, "[:nick]", "cmd", "receiver_list", ":msg");
		int add = 0;
		if (has_preffix)
			++add;
		message = line.at(2 + add);
	}
	else if (command == "NOTICE")
	{
		check_format(line, "[:nick]", "cmd", "receiver_list", ":msg");
		int add = 0;
		if (has_preffix)
			++add;
		message = line.at(2 + add);
	}
// ---------------------------- User-Based Queries -----------------------------
	else if (command == "WHO")
	{
		check_format(line, "[:nick]", "cmd", "target");
		int add = 0;
		if (has_preffix)
			++add;
		target = line.at(1 + add);
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

std::string
tlucanti::IRCParser::exec(const Socket &client)
{
	if (has_preffix)
	{
		user = database[prefix];
		if (user == nullptr)
			return IRC::ERR_NOSUCHNICK(database[prefix], prefix, "user with nickname");
	}
	else
		user = database[client];

	init();
	if (command.empty())
		return "";
	parse();
//	if ((command == "PRIVMSG" or command == "NOTICE") and dcc)
//		return compose_dcc(message);
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
	else if (command == "ERROR")
		return compose_error();
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

tlucanti::IRCParser::arg_list_type &
tlucanti::IRCParser::split_string(const std::string &str, arg_list_type &out)
{
	std::stringstream	ss(str);

	if (has_preffix)
		ss >> prefix;
	while (true)
	{
		std::string next;
		ss >> next;
		if (next.empty())
			break ;
		if (next.at(0) == ':')
		{
			std::string remaining;
			std::getline(ss, remaining);
			next += remaining;
		}
		out.push_back(next);
	}
	return out;
}

void
tlucanti::IRCParser::check_format_single(const std::string &arg, const std::string &format)
{
	arg_list_type _line;
	_line.push_back(arg);
	arg_list_type _fmt;
	_fmt.push_back(format);
	check_format__macro(_line, _fmt);
}

void
tlucanti::IRCParser::check_format__macro(arg_list_type &_line, arg_list_type &format)
{
	arg_list_type::iterator format_i = format.begin();
	arg_list_type::iterator line_i = _line.begin();

	for (;format_i != format.end(); ++format_i, ++line_i)
	{
		if (line_i == _line.end())
		{
			if (*format_i == "cmd")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected command"));
			else if (*format_i == "pass")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "excepted password"));
			else if (*format_i == "nick")
				throw IRCParserException(IRC::ERR_NONICKNAMEGIVEN(*user));
			else if (*format_i == "str")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "excepted argument"));
			else if (*format_i == "chan")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "excepted channel"));
			else if (*format_i == ":msg")
				throw IRCParserException(IRC::ERR_NOTEXTTOSEND(*user));
			else if (*format_i == "chan_list")
				throw IRCParserException(IRC::ERR_NORECIPIENT(*user, "expected channel list"));
			else if (*format_i == "user_list")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected user list"));
			else if (*format_i == "receiver_list")
				throw IRCParserException(IRC::ERR_NORECIPIENT(*user, "expected target list"));
			else if (*format_i == "target")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected target"));
			else if (format_i->at(0) != '[')
				ABORT("invalid format specifier", *format_i);
			else
				break ;
		}
		if (*format_i == "cmd") {}
		else if (*format_i == "pass")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected password, not message suffix"));
			if (not contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "message can contain only printable characters with no spaces"));
		}
		else if (*format_i == "nick")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "excepted nickname, not message suffix"));
			if (not contains_only(*line_i, IRCParser::NICKNAME))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "nickname can contain only digits, letters or special symbols -[]\\`^{}\n"));
		}
		else if (*format_i == "str")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected argument, not message suffix"));
			if (not contains_only(*line_i, IRCParser::ALNUM))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "string argumnet can contain only digin and letters"));
		}
		else if (*format_i == "chan")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected channel name, not message suffix"));
			if (line_i->at(0) != '#' and line_i->at(0) != '&')
				throw IRCParserException(IRC::ERR_NOSUCHCHANNEL(*user, channel, "channel name should start with # or &"));
			if (not contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "channel name can contain only printable characters with no spaces"));
		}
		else if (*format_i == ":msg")
		{
			if (line_i->size() >= 3 and line_i->at(1) == 1 and line_i->at(line_i->size() - 1) == 1) // DCC message
			{
				line_i->erase(0, 1);
				dcc = true;
				return ;
			}
			if (not contains_only(*line_i, IRCParser::PRINTABLESPACE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "message can contain only printable characters"));
			if (line_i->at(0) == ':')
				line_i->erase(0, 1);
		}
		else if (*format_i == "chan_list" or *format_i == "[chan_list]")
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			arg_list_type::iterator it=split_list.begin();
			for (; it != split_list.end(); ++it)
			{
				check_format_single(*it, "chan");
				chan_list.push_back(*it);
			}
		}
		else if (*format_i == "user_list" or *format_i == "[user_list]")
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			arg_list_type::iterator it=split_list.begin();
			for (; it != split_list.end(); ++it)
			{
				check_format_single(*it, "nick");
				user_list.push_back(*it);
			}
		}
		else if (*format_i == "receiver_list")
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
				check_format_single(*it, "target");
		}
		else if (*format_i == "target")
		{
			if (line_i->at(0) == '#' or line_i->at(0) == '&')
			{
				check_format_single(*line_i, "chan");
				chan_list.push_back(*line_i);
			}
			else
			{
				check_format_single(*line_i, "nick");
				user_list.push_back(*line_i);
			}
		}
		else if (*format_i == "user_sequence")
		{
			for (; line_i != line.end(); ++line_i)
			{
				check_format_single(*line_i, "nick");
				user_list.push_back(*line_i);
			}
			return ;
		}
		else if (*format_i == "[:nick]")
		{
			if (line_i->at(0) != ':')
			{
				--line_i;
				continue ;
			}
			arg_list_type check;
			line_i->erase(0, 1);
			check.push_back(*line_i);
			check_format(check, "nick");
		}
		else if (*format_i == "[pass_list]")
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			arg_list_type::iterator it=split_list.begin();
			for (; it != split_list.end(); ++it)
			{
				arg_list_type split_elem;
				split_elem.push_back(*it);
				check_format(split_elem, "pass");
				pass_list.push_back(*it);
			}
		}
		else if (*format_i == "[mode_list]")
		{
			while (line_i != _line.end())
				modes_list.push_back(*line_i++);
			--line_i;
		}
		else if (*format_i == "[:msg]")
		{
			has_suffix = true;
			arg_list_type check;
			check.push_back(*line_i);
			check_format(check, ":msg");
		}
		else if (*format_i == "[str]")
		{
			++has_suffix;
			arg_list_type check;
			check.push_back(*line_i);
			check_format(check, "str");
		}
		else if ((*format_i).at(0) != '[')
			ABORT("unknown specifier", *format_i);
	}
	if (line_i != _line.end())
		throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "extra tokens at the end of command ignored: " + *line_i));
}
