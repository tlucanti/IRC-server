/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:35:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 13:56:19 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

const char *tlucanti::IRCParser::PRINTABLE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~";
const char *tlucanti::IRCParser::PRINTABLESPACE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~ ";
const char *tlucanti::IRCParser::ALNUM = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *tlucanti::IRCParser::NICKNAME = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-+=^[](){}";

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
		has_suffix(false),
		has_preffix(false),
		dcc(false),
		user(nullptr),
		_raw_command(raw_command) {}

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

	if (line.empty() or (line.size() == 1 and has_preffix))
		throw IRCParserException("");

	arg_list_type::iterator it = line.begin();
	command = *it; // iterator will valid because line.size() >= 1
	while (++it != line.end())
		arguments.push_back(*it);
}

tlucanti::IRCParser::arg_list_type &
tlucanti::IRCParser::split_string(const std::string &str, arg_list_type &out)
{
	std::stringstream	ss(str);

	if (has_preffix)
	{
		ss >> prefix;
		prefix.erase(0, 1); // remove ':' symbol
	}
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
tlucanti::IRCParser::
check_format__macro(arg_list_type &_line, arg_list_type &format)
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
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected password"));
			else if (*format_i == ":pass")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected password"));
			else if (*format_i == "nick")
				throw IRCParserException(IRC::ERR_NONICKNAMEGIVEN(*user));
			else if (*format_i == "str")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected argument"));
			else if (*format_i == "chan")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected channel"));
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
			else if (*format_i == "any")
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected argument"));
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
		else if (*format_i == ":pass" or *format_i == "[:pass]")
		{
			if (format_i->at(0) == '[')
				++has_suffix;
			if (line_i->at(0) == ':')
				line_i->erase(0, 1);
			check_format_single(*line_i, "pass");
		}
		else if (*format_i == "nick")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected nickname, not message suffix"));
			if (not contains_only(*line_i, IRCParser::NICKNAME))
				throw IRCParserException(IRC::ERR_ERRONEUSNICKNAME(*user));
		}
		else if (*format_i == "str")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected argument, not message suffix"));
			if (not contains_only(*line_i, IRCParser::ALNUM))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "string argument may contain only digit and letters"));
		}
		else if (*(format_i) == "any")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected argument, not message suffix"));
			if (not contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "argument may contain only printable characters"));
		}
		else if (*format_i == "chan")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRC::ERR_NEEDMOREPARAMS(*user, command, "expected channel name, not message suffix"));
			if (line_i->at(0) != '#' and line_i->at(0) != '&')
				throw IRCParserException(IRC::ERR_NOSUCHCHANNEL(*user, *line_i, "channel name should start with # or &"));
			if (not contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "channel name may contain only printable characters with no spaces"));
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
				throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "message may contain only printable characters"));
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
			++has_suffix;
			while (line_i != _line.end())
				modes_list.push_back(*line_i++);
			--line_i;
		}
		else if (*format_i == "[:msg]")
		{
			++has_suffix;
			check_format_single(*line_i, ":msg");
		}
		else if (*format_i == "[str]")
		{
			++has_suffix;
			check_format_single(*line_i, "str");
		}
		else if (*format_i == "[any]")
		{
			++has_suffix;
			check_format_single(*line_i, "any");
		}
		else if ((*format_i).at(0) != '[')
			ABORT("unknown specifier", *format_i);
	}
	if (line_i != _line.end())
		throw IRCParserException(IRC::compose_message(nullptr, "NOTICE", *user, "extra tokens at the end of command"));
}
