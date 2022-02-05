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
		IRCParserException(const char *message)
				: _message(message) {}
		IRCParserException(const std::string &message)
				: _message(message.c_str()) {}
		const char *what() const noexcept override { return _message; }

	private:
		const char *_message;
	};
}

#endif /* IRCPARSER_EXCEPTION_HPP */
