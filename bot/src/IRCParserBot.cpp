/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParserBot.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 12:08:10 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/21 15:37:16 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCParserBot.hpp"

void
tlucanti::IRCParserBot::parse()
{
	IRCParser::init();

	std::string _nickname;
	if (has_preffix)
	{
		std::size_t sg = prefix.find('!');
		if (sg == std::string::npos)
			_nickname = "";
		else
			_nickname = prefix.substr(0, sg);
	}
	IRCParser::parse();
	nickname = _nickname;
}

__WUR
std::string
tlucanti::IRCParserBot::get_target() const
{
	if (not user_list.empty())
		return user_list.front();
	return chan_list.front();
}
