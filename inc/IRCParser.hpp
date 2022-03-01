/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 14:20:18 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 13:51:02 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

# include <string>
# include <list>
# include <vector>
# include <sstream>
# include <set>
# include <iostream>
# include <sys/stat.h>

# include "defs.h"
# include "IRCcodes.h"
# include "Database.hpp"
# include "IRCParserException.hpp"
# include "Exception.hpp"
# include "parser_utils.hpp"
# include "IRCrpl.hpp"

namespace tlucanti
{
	#define check_format(line, ...) do \
	{ \
		const char *_fmt[] = {__VA_ARGS__}; \
		std::vector<std::string> _vec(_fmt, \
			_fmt + sizeof(_fmt) / sizeof(_fmt[0])); \
		check_format__macro(line, _vec); \
	} while (false)

	class IRCParser
	{
	public:
		typedef std::vector<std::string>	arg_list_type;

		IRCParser(const std::string &raw_command);
		std::string exec(const Socket &client);

	protected:
		std::string		prefix;
		std::string		command;

		arg_list_type	arguments;
		arg_list_type	line;

		void init();
		void parse();

		std::string nickname;
		std::string password;
		std::string realname;
		std::string message;
		std::string channel;
		std::string mode;
		std::string target;
		arg_list_type chan_list;
		arg_list_type user_list;
		arg_list_type pass_list;
		arg_list_type modes_list;
		int has_suffix;
		bool has_preffix;
		bool dcc;

	private:
		User *user;

		__WUR std::string compose_cap() const;
		__WUR std::string compose_pass() const;
		__WUR std::string compose_nick() const;
		__WUR std::string compose_user() const;
		__WUR std::string compose_ping() const;
		__WUR std::string compose_pong() const;
		__WUR std::string compose_oper() const;
		__WUR std::string compose_quit();

		__WUR std::string compose_join();
		__WUR std::string compose_part() const;
		__WUR std::string compose_topic() const;
		__WUR std::string compose_names();
		__WUR std::string compose_names_single() const;
		__WUR std::string compose_list();
		__WUR std::string compose_list_single() const;
		__WUR std::string compose_invite() const;
		__WUR std::string compose_kick();

		__WUR std::string compose_motd() const;
		__WUR std::string compose_version() const;
		__WUR std::string compose_admin() const;
		__WUR std::string compose_time() const;
		__WUR std::string compose_help();
		__WUR std::string compose_info() const;
		__WUR std::string compose_mode();
		__WUR std::string compose_mode_single();

		__WUR std::string compose_msg(const char *type);
		void compose_msg_usr_single(const char *type) const;
		__WUR std::string compose_msg_chan_single(const char *type) const;
		__WUR std::string compose_privmsg();
		__WUR std::string compose_notice();

		__WUR std::string compose_who() const;
		__WUR std::string compose_ison();

		__WUR std::string compose_kill() const;
		__WUR std::string compose_restart() const;
		__WUR std::string compose_squit() const;

		__WUR std::string compose_dcc(__UNUSED const std::string &message);

		std::vector<std::string> &split_string(const std::string &str, arg_list_type &out);
		void check_format_single(const std::string &arg, const std::string &format);
		void check_format__macro(arg_list_type &_line, arg_list_type &_fmt);

		static const char *PRINTABLE;
		static const char *PRINTABLESPACE;
		static const char *ALNUM;
		static const char *NICKNAME;
		std::string		_raw_command;

	__DELETED_MEMBERS:
		IRCParser() __DELETE
		IRCParser(const IRCParser &) __DELETE
		IRCParser &operator =(const IRCParser &) __DELETE
	};
}

#endif /* IRCPARSER_HPP */
