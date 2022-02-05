/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 14:20:18 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 14:26:16 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP

#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <set>

#include "defs.h"
#include "IRCcodes.h"
#include "IRCParserException.hpp"
#include "IRCException.hpp"

namespace tlucanti
{
	#define check_format(line, ...) ({const char *_fmt[] = {__VA_ARGS__}; _check_format__macro(line, _fmt);})

	class IRCParser
	{
	public:
		typedef std::vector<std::string>	arg_list_type;

		IRCParser(const std::string &raw_command)
				: _raw_command(raw_command) {}

		void init();
		void parse();

		std::string		prefix;
		std::string		command;
		arg_list_type	arguments;
		arg_list_type	line;
	private:
		static std::vector<std::string> &split_string(const std::string &str, std::vector<std::string> &out);
		__WUR void _check_format__macro(const std::vector<std::string> &_line, const char **_char_fmt);
		__WUR bool contains_only(const std::string &str, const std::string &format);

		static const char *PRINTABLE;
		static const char *LETTERS;
		std::string		_raw_command;
	};

	void split(const std::string &str, std::vector<std::string> out, char c);
}

#endif /* IRCPARSER_HPP */
