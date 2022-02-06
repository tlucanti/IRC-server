/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 14:00:44 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:16 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_EXCEPTION_HPP
# define IRC_EXCEPTION_HPP

#include <stdexcept>
#include <cstring>
#include <string>

#include "defs.h"

namespace tlucanti
{
	class IRCException : public std::exception
	{
	public:
		IRCException(const std::string &parent,
			const std::string &message, int error);
		IRCException(const std::string &parent,
			const std::string &message);
		IRCException(const std::string &parent,
			const std::string &message, const std::string &error);
		~IRCException() noexcept override __DEFAULT
		__WUR const char *what() const noexcept override;

	protected:
		std::string _message;
		std::string _parent;
	};
}

#endif	/* IRC_EXCEPTION_HPP */
