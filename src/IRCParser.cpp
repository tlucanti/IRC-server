/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:35:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:46 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParser.hpp"

const char *tlucanti::IRCParser::PRINTABLE = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&'()*+-./:;<=>?@[\]^_`{|}~)";
const char *tlucanti::IRCParser::PRINTABLESPACE = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!"#$%&'()*+-./:;<=>?@[\]^_`{|}~ )";
const char *tlucanti::IRCParser::LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *tlucanti::IRCParser::NICKNAME = R"(0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-[]\`^{}*)";

void
tlucanti::IRCParser::init()
{
	_raw_command = strip(_raw_command);
//	_raw_command = squeeze(_raw_command); // multiple space -> single space
//	_raw_command = replace(_raw_command, ", ", ","); // , ', ' -> ','
	split_string(_raw_command, line);
	arg_list_type::iterator it = line.begin();
	has_suffix = false;

	if (line.empty())
		throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS,
			"command body is empty");
	if (it->at(0)== ':')
	{
		if (line.size() == 1)
			throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS,
				"command body is empty");
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
	if (command == "PASS")
	{
		check_format(line, "cmd", "str");
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
		hostname = line.at(2 + add);
		servername = line.at(3 + add);
		realname = line.at(4 + add);
	}
	else if (command == "OPER")
	{
		check_format(line, "[:nick]", "cmd", "nick", "str");
		int add = 0;
		if (not prefix.empty())
			++add;
		nickname = line.at(1 + add);
		password = line.at(2 + add);
	}
	else if (command == "QUIT")
	{
		check_format(line, "[:nick]", "cmd", ":msg");
		int add = 0;
		if (not prefix.empty())
			++add;
		message = line.at(1 + add);
	}
	else if (command == "JOIN")
		check_format(line, "[:nick]", "cmd", "chan_list", "[str_list]");
	else if (command == "PART")
		check_format(line, "[:nick]", "cmd", "chan_list");
	else if (command == "MODE")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement mode command");
	else if (command == "TOPIC")
	{
		check_format(line, "[:nick]", "cmd", "chan", ":msg");
		int add = 0;
		if (not prefix.empty())
			++add;
		channel = line.at(1 + add);
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
	else if (command == "VERSION")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement version command");
	else if (command == "TIME")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement time command");
	else if (command == "INFO")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement info command");
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
		throw IRCException("[tlucanti::IRCParser::parse]", "implement pong command");
	else if (command == "RESTART")
		throw IRCException("[tlucanti::IRCParser::parse]", "implement restart command");
	else
		throw IRCParserException(IRCcodes::ERR_UNKNOWNCOMMAND,
			"unknown command `" + command + "`");
}

std::string
tlucanti::IRCParser::exec(const Socket &client, Database &database)
{
	User &user = database[client];
	if (command == "PASS")
	{
		user.check_perm("p-");
		if (password != tlucanti::server_password)
			throw IRCParserException(IRCcodes::ERR_PASSWDMISMATCH,
				"password incorrect");
		user.make_pass();
		return "authentication success";
	}
	else if (command == "NICK")
	{
		user.check_perm("p+");
		user.make_nickname(nickname);
		if (database.make_edge(nickname, client))
			throw IRCParserException(IRCcodes::ERR_NICKNAMEINUSE,
				"user with nickname `" + nickname + "` already exist");
		return "nickname changed";
	}
	else if (command == "USER")
	{
		user.check_perm("n+");
		user.make_user(nickname, hostname, servername, realname);
		return "authorization success";
	}
	else if (command == "OPER")
	{

	}
	else if (command == "QUIT")
	{

	}
	else if (command == "JOIN")
	{

	}
	else if (command == "PART")
	{

	}
	else if (command == "MODE")
	{

	}
	else if (command == "TOPIC")
	{

	}
	else if (command == "NAMES")
	{

	}
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
	else if (command == "PRIVMSG")
	{
		user.check_perm("u+");
		for (arg_list_type::iterator it=user_list.begin();
			it != user_list.end(); ++it)
		{
			User &cli = database[*it];
			if (cli == User::nil)
				throw IRCParserException(IRCcodes::ERR_NOSUCHNICK,
					"user with nickname `" + *it + "` does not exist");
			cli.send_message(message);
//			cli.send_message(compose_message(message));
		}
		for (arg_list_type::iterator it=chan_list.begin();
			it != chan_list.end(); ++it)
		{
			Channel &chan = database.get_channel(*it);
			if (chan == Channel::nil)
				throw IRCParserException(IRCcodes::ERR_NOSUCHNICK,
					"channel with name `" + *it + "` does not exist");
			chan.send_message(message);
		}
		return "";
	}
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
	throw IRCException("[internal error]", "IRCParser::exec", "command `" + command + "` not implemented");
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
tlucanti::IRCParser::_check_format__macro(const arg_list_type &_line, arg_list_type &format)
{
	arg_list_type::iterator format_i = format.begin();
	arg_list_type::const_iterator line_i = _line.begin();

	while (format_i != format.end())
	{
		std::cout << "checking " << *line_i << " == " << *format_i << std::endl;
		if (line_i == _line.end())
		{
			if (*format_i == "cmd")
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS,
					"expected command");
			else if (*format_i == "str")
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS,
					"excepted argument");
			else if (*format_i == "nick")
				throw IRCParserException(IRCcodes::ERR_NONICKNAMEGIVEN,
					"excepted nickname");
			else if (*format_i == "chan")
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS,
					"excepted channel");
			else if (*format_i == ":msg")
				throw IRCParserException(IRCcodes::ERR_NOTEXTTOSEND, "expected message");
			else if (*format_i == "chan_list")
				throw IRCParserException(IRCcodes::ERR_NORECIPIENT, "expected channel list");
			else if (*format_i == "receiver_list")
				throw IRCParserException(IRCcodes::ERR_NORECIPIENT, "expected channel or user list");
			else if (format_i->at(0) != '[')
				throw IRCException("[internal error]", "tlucanti::IRCParser::_check_format__macro (invalid format specifier)", *format_i);
		}
		if (*format_i == "cmd") {}
		else if (*format_i == "str")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS, "expected argument, not message suffix");
			if (not IRCParser::contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRCcodes::ERR_UNKNOWNCOMMAND, "message can contain only printable characters with no spaces");
		}
		else if (*format_i == "nick")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS, "excepted nickname, not message suffix");
			if (not contains_only(*line_i, IRCParser::NICKNAME))
				throw IRCParserException(IRCcodes::ERR_ERRONEUSNICKNAME, "nickname can contain only digits, letters or special symbols -[]\\`^{}");
		}
		else if (*format_i == "chan")
		{
			if (line_i->at(0) == ':')
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS, "expected channel name, not message suffix");
			if (line_i->at(0) != '#' and line_i->at(0) != '&')
				throw IRCParserException(IRCcodes::ERR_NOSUCHCHANNEL, "channel name should start with # or &");
			if (not contains_only(*line_i, IRCParser::PRINTABLE))
				throw IRCParserException(IRCcodes::ERR_NOSUCHCHANNEL, "channel name should contain only printable characters with no spaces");
		}
		else if (*format_i == ":msg")
		{
			if (line_i->at(0) != ':')
				throw IRCParserException(IRCcodes::ERR_NEEDMOREPARAMS, "message suffix should start with colon");
			if (not contains_only(*line_i, IRCParser::PRINTABLESPACE))
				throw IRCParserException(IRCcodes::ERR_UNKNOWNCOMMAND, "message should contain only printable characters with no spaces");
		}
		else if (*format_i == "chan_list")
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
				if (it->at(0) == '#' or it->at(0) == '&')
				{
					check_format(split_elem, "chan");
					chan_list.push_back(*it);
				}
				else
				{
					check_format(split_elem, "nick");
					user_list.push_back(*it);
				}
			}
		}
		else if (*format_i == "[:nick]" and line_i != _line.end())
		{
			if (line_i->at(0) != ':')
			{
				++format_i;
				continue ;
			}
			arg_list_type check;
			check.push_back((*line_i).substr(1));
			check_format(check, "nick");
		}
		else if (*format_i == "[str_list]" and line_i != _line.end())
		{
			arg_list_type split_list;
			split(*line_i, split_list, ',');
			for (arg_list_type::iterator it=split_list.begin(); it != split_list.end(); ++it)
			{
				arg_list_type split_elem;
				split_elem.push_back(*it);
				check_format(split_elem, "str");
				pass_list.push_back(*it);
			}
		}
		else if (*format_i == "[chan_list]" and line_i != _line.end())
		{
			arg_list_type check;
			check.push_back((*line_i).substr(1));
			check_format(check, "chan_list");
		}
		else if (*format_i == "[:msg]" and line_i != _line.end())
		{
			has_suffix = true;
			arg_list_type check;
			check.push_back(*line_i);
			check_format(check, ":msg");
		}
		else
			throw IRCException("[internal error]", "tlucanti::IRCParser::_check_format__macro (invalid format specifier)", *format_i);
		++format_i;
		++line_i;
	}
	if (line_i != _line.end())
		throw IRCParserException(IRCcodes::ERR_UNKNOWNCOMMAND, "extra tokens at end of command");
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
