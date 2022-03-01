/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 14:00:44 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/28 16:50:42 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_EXCEPTION_HPP
# define IRC_EXCEPTION_HPP

# include <stdexcept>
# include <cstring>
# include <string>
# include <sstream>

# include "defs.h"

# define ABORT(msg, arg) throw tlucanti::Exception(__PRETTY_FUNCTION__, __LINE__, msg, arg)

namespace tlucanti
{
	class Exception : public std::exception
	{
	public:
		Exception(const char *func, int line, const char *message, const std::string &arg);
		Exception(const std::string &parent,
			const std::string &message, int error);
		Exception(const std::string &parent,
			const std::string &message);
		Exception(const std::string &parent,
			const std::string &message, const std::string &error);
		~Exception() noexcept override __DEFAULT
		Exception(const Exception &cpy);
		__WUR const char *what() const noexcept override;

	protected:
		std::string _message;
		std::string total_errors() const;
	};
}

#endif	/* IRC_EXCEPTION_HPP */
