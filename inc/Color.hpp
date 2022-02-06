/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 16:58:26 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:10 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
# define COLOR_HPP

# include <iostream>
# include <string>

# include "parser_utils.hpp"
# include "defs.h"

namespace tlucanti
{
	class color
	{
	public:
		static const char *k; // black
		static const char *r; // red
		static const char *g; // green
		static const char *y; // yellow
		static const char *b; // blue
		static const char *p; // purple
		static const char *c; // cyan
		static const char *w; // white
		static const char *s; // reset

		static const char *tlucanti;

		color() __DEFAULT

	__DELETED_MEMBERS:
		color(const color &) __DELETE
		color &operator =(const color &) __DELETE
	};

	const char *color::k  = "\033[1;90m";
	const char *color::r  = "\033[1;91m";
	const char *color::g  = "\033[1;92m";
	const char *color::y  = "\033[1;93m";
	const char *color::b  = "\033[1;94m";
	const char *color::p  = "\033[1;95m";
	const char *color::c  = "\033[1;96m";
	const char *color::w  = "\033[1;97m";
	const char *color::s  = "\033[0m";

	const char *color::tlucanti   = (char *) "\033[19;1;96;21;6m";
}

const tlucanti::color &operator <<(const tlucanti::color &out, const std::string &message)
{
	std::string colored = message;
	colored = tlucanti::replace(colored, "[k]", tlucanti::color::k);
	colored = tlucanti::replace(colored, "[r]", tlucanti::color::r);
	colored = tlucanti::replace(colored, "[g]", tlucanti::color::g);
	colored = tlucanti::replace(colored, "[y]", tlucanti::color::y);
	colored = tlucanti::replace(colored, "[b]", tlucanti::color::b);
	colored = tlucanti::replace(colored, "[p]", tlucanti::color::p);
	colored = tlucanti::replace(colored, "[c]", tlucanti::color::c);
	colored = tlucanti::replace(colored, "[w]", tlucanti::color::w);
	colored = tlucanti::replace(colored, "[]", tlucanti::color::s);
	std::cout << colored;
	return out;
}

const tlucanti::color &operator <<(const tlucanti::color &out, const std::exception &exc)
{
	std::string msg = exc.what();
	return out << msg;
}

#endif  // COLOR_HPP
