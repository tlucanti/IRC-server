/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 14:20:18 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/08 11:08:09 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <set>
#include <iostream>

#include "defs.h"
#include "IRCcodes.h"
#include "Database.hpp"
#include "IRCParserException.hpp"
#include "IRCException.hpp"
#include "parser_utils.hpp"
#include "IRCrpl.hpp"

namespace tlucanti
{
	extern std::string server_name;
	extern std::string server_password;
	extern std::string server_begining;
	extern std::string server_version;
	extern std::string server_oper_login;
	extern std::string server_oper_password;
	extern Database database;
}

namespace tlucanti
{
	#define check_format(line, ...) do \
	{ \
		const char *_fmt[] = {__VA_ARGS__}; \
		std::vector<std::string> _vec(_fmt, \
			_fmt + sizeof(_fmt) / sizeof(_fmt[0])); \
		_check_format__macro(line, _vec); \
	} while (false)

	class IRCParser
	{
	public:
		typedef std::vector<std::string>	arg_list_type;

		IRCParser(const std::string &raw_command)
				: _raw_command(raw_command), has_suffix(false), user(nullptr) {}

		std::string exec(const Socket &client);
		std::string		prefix;
		std::string		command;

		arg_list_type	arguments;
		arg_list_type	line;

	private:
		void init();
		void parse();

		std::string nickname;
		std::string password;
		std::string hostname;
		std::string servername;
		std::string realname;
		std::string message;
		std::string channel;
		arg_list_type chan_list;
		arg_list_type user_list;
		arg_list_type pass_list;
		bool has_suffix;
		User *user;

		__WUR std::string compose_cap() const;
		__WUR std::string compose_pass() const;
		__WUR std::string compose_nick() const;
		__WUR std::string compose_user() const;
		__WUR std::string compose_oper() const;

		static std::vector<std::string> &split_string(const std::string &str, arg_list_type &out);
		void _check_format__macro(const arg_list_type &_line, arg_list_type &_fmt);
		__WUR bool contains_only(const std::string &str, const std::string &format);

		static const char *PRINTABLE;
		static const char *PRINTABLESPACE;
		static const char *LETTERS;
		static const char *NICKNAME;
		std::string		_raw_command;

	__DELETED_MEMBERS:
		IRCParser() __DELETE
		IRCParser(const IRCParser &) __DELETE
		IRCParser &operator =(const IRCParser &) __DELETE

	};
}

#endif /* IRCPARSER_HPP */
