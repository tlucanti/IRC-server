/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParserBot.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 12:08:05 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 15:43:11 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_BOT_HPP
# define IRCPARSER_BOT_HPP

# include "../../inc/IRCParser.hpp"

namespace tlucanti
{
	class IRCParserBot : public IRCParser
	{
	public:
		IRCParserBot(const std::string &raw_command)
			: IRCParser(raw_command) {}

		void parse();

		__WUR inline std::string get_origin() const { return nickname; }
		__WUR inline std::string get_command() const { return command; }
		__WUR std::string get_target() const;
		__WUR inline std::string get_content() const { return message; }
	};
}

#endif /* IRCPARSER_BOT_HPP */
