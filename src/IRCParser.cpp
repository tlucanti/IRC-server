/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:35:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/10 19:12:42 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

const char *tlucanti::IRCParser::PRINTABLE = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&'()*+-./:;<=>?@[\]^_`{|}~)";
const char *tlucanti::IRCParser::PRINTABLESPACE = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&'()*+-./:;<=>?@[\]^_`{|}~ )";
const char *tlucanti::IRCParser::LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *tlucanti::IRCParser::NICKNAME = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-[]\`^{}*)";

namespace tlucanti
{
	extern Database database;
}

void
tlucanti::IRCParser::init()
{
	_raw_command = replace(_raw_command, "\r", "");
	_raw_command = replace(_raw_command, "\n", "");
	_raw_command = strip(_raw_command);
//	_raw_command = squeeze(_raw_command); // multiple space -> single space
//	_raw_command = replace(_raw_command, ", ", ","); // , ', ' -> ','
	split_string(_raw_command, line);
	arg_list_type::iterator it = line.begin();
	has_suffix = false;

	if (line.empty())
		throw IRCParserException("");
	if (it->at(0)== ':')
	{
		if (line.size() == 1)
			throw IRCParserException("");
		prefix = *it;
		++it;
	}
	command = *it;
	while (++it != line.end())
		arguments.push_back(*it);
}


void
tlucanti::IRCParser::parse()
{
	typedef const char *fmt_t[];

	// reformat this function and remove if/else
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
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
		if (not prefix.empty())
			++add;
		nickname = line.at(1 + add);
	}
	else if (command == "USER")
	{
		check_format(line, "[:nick]", "cmd", "nick", "nick", "nick", ":msg");
		int add = 0;
		if (not prefix.empty())
			++add;
		nickname = line.at(1 + add);
		realname = line.at(4 + add);
	}
	else if (command == "OPER")
	{
		check_format(line, "[:nick]", "cmd", "nick", "pass");
		int add = 0;
		if (not prefix.empty())
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
	else if (command == "JOIN")
		check_format(line, "[:nick]", "cmd", "chan_list", "[pass_list]");
	else if (command == "PART")
	{
		check_format(line, "[:nick]", "cmd", "chan_list", "[:msg]");
		int add = 0;
		if (not prefix.empty())
			++add;
		if (has_suffix)
			message = line.at(2 + add);
	}
	else if (command == "TOPIC")
	{
		check_format(line, "[:nick]", "cmd", "chan", "[:msg]");
		int add = 0;
		if (not prefix.empty())
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
		if (not prefix.empty())
			++add;
		nickname = line.at(1 + add);
		channel = line.at(2 + add);
	}
	else if (command == "KICK")
	{
		check_format(line, "[:nick]", "cmd", "chan", "nick", "[:msg]");
		int add = 0;
		if (not prefix.empty())
			++add;
		channel = line.at(1 + add);
		nickname = line.at(2 + add);
		if (has_suffix)
			message = line.at(3 + add);
	}
	else if (command == "MOTD")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement motd command");
	else if (command == "VERSION")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement version command");
	else if (command == "ADMIN")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement admin command");
	else if (command == "HELP")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement admin command");
	else if (command == "INFO")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement info command");
	else if (command == "MODE")
	{
		check_format(line, "[:nick]", "cmd", "target", "[str]", "[mode_list]");
		int add = 0;
		if (not prefix.empty())
			++add;
		target = line.at(1 + add);
		if (has_suffix >= 1)
			mode = line.at(2 + add);
	}
	else if (command == "WHO")
	{
		check_format(line, "cmd", "target");
		target = line.at(1);
	}
	else if (command == "PRIVMSG")
	{
		check_format(line, "[:nick]", "cmd", "receiver_list", ":msg");
		int add = 0;
		if (not prefix.empty())
			++add;
		message = line.at(2 + add);
	}
	else if (command == "KILL")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement kill command");
	else if (command == "PING")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement ping command");
	else if (command == "PONG")
	{
		check_format(line, "cmd", "pass");
	}
	else if (command == "RESTART")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement restart command");
	else if (command == "TIME")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement time command");
	else
		throw IRCParserException(IRC::ERR_UNKNOWNCOMMAND(*user, command));
}

std::string
tlucanti::IRCParser::exec(const Socket &client)
{
	if (not prefix.empty())
	{
		user = database[prefix];
		if (user == nullptr)
			return IRC::ERR_NOSUCHNICK(database[client], prefix, "user with nickname");
	}
	else
		user = database[client];

	init();
	parse();

	if (command == "CAP")
		return compose_cap();
	if (command == "PASS")
		return compose_pass();
	else if (command == "NICK")
		return compose_nick();
	else if (command == "USER")
		return compose_user();
	else if (command == "OPER")
		return compose_oper();
	else if (command == "QUIT")
		return compose_quit();
	else if (command == "JOIN")
		return compose_join();
	else if (command == "PART")
		return compose_part();
	else if (command == "MODE")
		return compose_mode();
	else if (command == "TOPIC")
		return compose_topic();
	else if (command == "NAMES")
		return compose_names();
	else if (command == "LIST")
	{

	}
	else if (command == "INVITE")
	{

	}
	else if (command == "KICK")
	{

	}
	else if (command == "VERSION")
	{

	}
	else if (command == "TIME")
	{

	}
	else if (command == "INFO")
	{

	}
	else if (command == "WHO")
		return compose_who();
	else if (command == "PRIVMSG")
		return compose_privmsg();
	else if (command == "KILL")
	{

	}
	else if (command == "PING")
	{

	}
	else if (command == "PONG")
	{

	}
	else if (command == "RESTART")
	{

	}
	else
	{

	}
	throw IRCException("[tlucanti::IRCParser::exec]", "IRCParser::exec", "command `" + command + "` not implemented");
}

tlucanti::IRCParser::arg_list_type &
tlucanti::IRCParser::split_string(const std::string &str, arg_list_type &out)
{
	std::stringstream	ss(str);
	bool was_prefix = false;

	while (true)
	{
		std::string next;
		ss >> next;
		if (next.empty())
			break ;
		if (was_prefix and next.at(0) == ':')
		{
			std::string last;
			std::getline(ss, last);
			next += last;
		}
		out.push_back(next);
		was_prefix = true;
	}
	return out;
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
			else if (*format_i == "chan")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "excepted channel"));
			else if (*format_i == ":msg")
				throw IRCParserException(IRC::ERR_NOTEXTTOSEND(*user));
			else if (*format_i == "chan_list")
				throw IRCParserException(IRC::ERR_NORECIPIENT(*user, "expected channel list"));
			else if (*format_i == "receiver_list")
				throw IRCParserException(IRC::ERR_NORECIPIENT(*user, "expected channel or user list"));
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
			if (not IRCParser::contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "message can contain only printable characters with no spaces"));
		}
		else if (*format_i == "nick")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "excepted nickname, not message suffix"));
			if (not contains_only(*line_i, IRCParser::NICKNAME))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "nickname can contain only digits, letters or special symbols -[]\\`^{}\n"));
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
			if (not contains_only(*line_i, IRCParser::PRINTABLESPACE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "message can contain only printable characters"));
			if (line_i->at(0) == ':')
				line_i->erase(0, 1);
		}
		else if (*format_i == "chan_list" or *format_i == "[chan_list]")
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
			{
				arg_list_type split_elem;
				split_elem.push_back(*it);
				check_format(split_elem, "chan");
				chan_list.push_back(*it);
			}
		}
		else if (*format_i == "receiver_list")
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
			{
				arg_list_type split_elem;
				split_elem.push_back(*it);
				check_format(split_elem, "target");
			}
		}
		else if (*format_i == "target")
		{
			arg_list_type split_elem;
			split_elem.push_back(*line_i);
			if (line_i->at(0) == '#' or line_i->at(0) == '&')
			{
				check_format(split_elem, "chan");
				chan_list.push_back(*line_i);
			}
			else
			{
				check_format(split_elem, "nick");
				user_list.push_back(*line_i);
			}
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
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
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
			line_i->erase(0, 1);
			check.push_back(*line_i);
			check_format(check, ":msg");
		}
		else if (*format_i == "[str]")
		{
			++has_suffix;
		}
		else if ((*format_i).at(0) != '[')
			ABORT("unknown specifier", *format_i);
	}
	if (line_i != _line.end())
		throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "extra tokens at the end of command: " + *line_i));
}

__WUR bool
tlucanti::IRCParser::contains_only(const std::string &str, const std::string &format)
{
	std::set<char> str_chars(str.begin(), str.end());
	std::set<char> format_chars(format.begin(), format.end());
	std::set<char> union_chars;
	std::set_union(str_chars.begin(), str_chars.end(), format_chars.begin(),
		format_chars.end(), std::inserter(union_chars, union_chars.begin()));
	if (union_chars.size() > format_chars.size())
		return false;
	return true;
}
