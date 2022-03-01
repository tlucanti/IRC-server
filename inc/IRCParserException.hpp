/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCParserException.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 22:34:45 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 22:59:35 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCPARSER_EXCEPTION_HPP
# define IRCPARSER_EXCEPTION_HPP

#include <stdexcept>
#include "defs.h"

namespace tlucanti
{
	class IRCParserException : public std::exception
	{
	public:
		IRCParserException(const std::string &response)
				: _message(response) {}
		IRCParserException() __DEFAULT
		__WUR const char *what() const noexcept override { return _message.c_str(); }
		~IRCParserException() noexcept override {}

	private:
		std::string _message;
	};
}

#endif /* IRCPARSER_EXCEPTION_HPP */
