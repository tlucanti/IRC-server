/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:35:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 14:26:44 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

const char *tlucanti::IRCParser::PRINTABLE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
const char *tlucanti::IRCParser::PRINTABLESPACE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ ";
const char *tlucanti::IRCParser::LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *tlucanti::IRCParser::NICKNAME = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-[]\\`^{}";

void
tlucanti::IRCParser::init()
{
	squeze_space(_raw_command); // multiple space -> single space, ', ' -> ','
	split_string(_raw_command, line);
	arg_list_type::iterator it = line.begin();

	if (line.empty())
		throw IRCParserException("command body is empty");
	if ((*it)[0] == ':')
	{
		if (line.size() == 1)
			throw IRCParserException("command body is empty");
		prefix = *it;
		++it;
	}
	command = *it;
	++it;
	std::copy(it, line.end(), arguments.begin());
}


void
tlucanti::IRCParser::parse()

{
	typedef const char *fmt_t[];

	// reformat this function and remove if/else
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
	if (command == "PASS")
		check_format(line, "[:nick]", "cmd", "str");
	else if (command == "NICK")
		check_format(line, "[:nick]", "cmd", "nick");
	else if (command == "USER")
		check_format(line, "[:nick]", "cmd", "nick", "str", "str", ":msg");
	else if (command == "OPER")
		check_format(line, "[:nick]", "cmd", "nick", "str");
	else if (command == "QUIT")
		check_format(line, "[:nick]", "cmd", ":msg");
	else if (command == "JOIN")
		check_format(line, "[:nick]", "cmd", "chan_list", "[str_list]");
	else if (command == "PART")
		check_format(line, "[:nick]", "cmd", "chan_list");
	else if (command == "MODE")
		throw std::runtime_error("[tlucanti::IRCParser::parse] implement mode command");
	else if (command == "TOPIC")
		check_format(line, "[:nick]", "cmd", "chan", ":msg");
	else if (command == "NAMES")
		check_format(line, "[:nick]", "cmd", "[chan_list]");
	else if (command == "LIST")
		check_format(line, "[:nick]",  "cmd", "[chan_list]");
	else if (command == "INVITE")
		check_format(line, "[:nick]", "cmd", "nick", "chan");
	else if (command == "KICK")
		check_format(line, "[:nick]", "cmd", "chan", "nick", "[:msg]");
	else
		throw IRCParserException("unknown command `" + command + "`");
}

tlucanti::IRCParser::arg_list_type &
tlucanti::IRCParser::split_string(const std::string &str, arg_list_type &out)
{
	std::stringstream	ss(str);
	bool was_prefix = false;

	while (ss)
	{
		std::string next;
		ss >> next;
		if (was_prefix)
		{
			if (next[0] == ':')
				std::getline(ss, next);
		}
		out.push_back(next);
		was_prefix = true;
	}
	return out;
}

__WUR void
tlucanti::IRCParser::_check_format__macro(const std::vector<std::string> &_line, const char **_char_fmt)
{
	int size = sizeof(_char_fmt) / sizeof(const char *);
	arg_list_type format(_char_fmt, _char_fmt + size);
	int format_i = 0;
	int line_i = 0;

	while (format_i < size)
	{
		if (line_i >= _line.size())
		{
			if (format.at(format_i) == "cmd")
				throw IRCParserException("expected command");
			else if (format.at(format_i) == "str")
				throw IRCParserException("excepted argument");
			else if (format.at(format_i) == "nick")
				throw IRCParserException("excepted nickname");
			else if (format.at(format_i) == "chan")
				throw IRCParserException("excepted channel");
			else if (format.at(format_i) == ":msg")
				throw IRCParserException("expected message");
			else if (format.at(format_i) == "chan_list")
				throw IRCParserException("expected channel list");
			else if (format.at(format_i)[0] != '[')
				throw IRCException("[internal error]", "tlucanti::IRCParser::_check_format__macro (invalid format specifier)", format.at(format_i));
		}
		if (format.at(format_i) == "cmd")
			continue ;
		else if (format.at(format_i) == "str")
		{
			if (_line.at(line_i)[0] == ':')
				throw IRCParserException("expected argument, not message suffix");
			if (not IRCParser::contains_only(_line.at(line_i), IRCParser::PRINTABLE))
				throw IRCParserException("message can contain only printable characters with no spaces");
		}
		else if (format.at(format_i) == "nick")
		{
			if (_line.at(line_i)[0] == ':')
				throw IRCParserException("excepted nickname, not message suffix");
			if (not std::isalpha(_line[0][0]))
				throw IRCParserException("first symbol of nickname should be letter");
			if (not contains_only(_line.at(line_i), IRCParser::NICKNAME))
				throw IRCParserException("nickname can contain only digits, letters or special symbols -[]\\`^{}");
		}
		else if (format.at(format_i) == "chan")
		{
			if (_line.at(line_i)[0] == ':')
				throw IRCParserException("expected channel name, not message suffix");
			if (_line.at(line_i)[0] != '#' and _line.at(line_i)[0] != '&')
				throw IRCParserException("channel name should start with # or &");
			if (not contains_only(_line.at(line_i), IRCParser::PRINTABLE))
				throw IRCParserException("channel name should contain only printable characters with no spaces");
		}
		else if (format.at(format_i) == ":msg")
		{
			if (_line.at(line_i)[0] != ':')
				throw IRCParserException("message suffix should start with colon");
			if (not contains_only(_line.at(line_i), IRCParser::PRINTABLESPACE))
				throw IRCParserException("message should contain only printable characters with no spaces");
		}
		else if (format.at(format_i) == "chan_list")
		{
			arg_list_type split_list;
			split(_line.at(line_i), split_list, ',');
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
				check_format(split_list, "chan");
		}
		else if (format.at(format_i) == "[:nick]" and line_i < _line.size())
		{
			if (_line.at(line_i)[0] != ':')
			{
				++format_i;
				continue ;
			}
			arg_list_type check;
			check.push_back(_line.at(line_i).substr(1));
			check_format(check, "nick");
		}
		else if (format.at(format_i) == "[str_list]" and line_i < _line.size())
		{
			arg_list_type split_list;
			split(_line.at(line_i), split_list, ',');
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
				check_format(split_list, "str");
		}
		else if (format.at(format_i) == "[chan_list]" and line_i < _line.size())
		{
			arg_list_type check;
			check.push_back(_line.at(line_i).substr(1));
			check_format(check, "chan_list");
		}
		else
			throw IRCException("[internal error]", "tlucanti::IRCParser::_check_format__macro (invalid format specifier)", format.at(format_i);
		++format_i;
		++line_i;
	}
	if (line_i != _line.size())
		throw IRCParserException("extra tokens at end of command");
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
