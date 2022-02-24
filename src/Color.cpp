/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 11:23:02 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/24 11:40:51 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Color.hpp"

const char *tlucanti::color::k  = "\033[1;90m";
const char *tlucanti::color::r  = "\033[1;91m";
const char *tlucanti::color::g  = "\033[1;92m";
const char *tlucanti::color::y  = "\033[1;93m";
const char *tlucanti::color::b  = "\033[1;94m";
const char *tlucanti::color::p  = "\033[1;95m";
const char *tlucanti::color::c  = "\033[1;96m";
const char *tlucanti::color::w  = "\033[1;97m";
const char *tlucanti::color::s  = "\033[0m";

const char *tlucanti::color::tlucanti   = (char *) "\033[19;1;96;21;6m";

namespace tlucanti
{
	const char *endl = "\033[0m\n";

	static void
	_FORMAT_COLOR(const char *fmt, const std::string &parent, int error, const std::string &msg)
	{
		tlucanti::cout << fmt << "[w] " << parent;
		if (error != 0)
			tlucanti::cout << ": []" << strerror(error);
		tlucanti::cout << ": [y]" << msg << "\n";
	}

	void WARN(const std::string &parent, int error, const std::string &msg)
	{ _FORMAT_COLOR("[y][WARN]", parent, error, msg); }

	void WARN(const std::string &parent, const std::string &msg)
	{ WARN(parent, 0, msg); }

	void ERROR(const std::string &parent, int error, const std::string &msg)
	{ _FORMAT_COLOR("[r][FAIL]", parent, error, msg); }

	void ERROR(const std::string &parent, const std::string &msg)
	{ ERROR(parent, 0, msg); }

	void INFO(const std::string &parent, int error, const std::string &msg)
	{ _FORMAT_COLOR("[b][INFO]", parent, error, msg); }

	void INFO(const std::string &parent, const std::string &msg)
	{ INFO(parent, 0, msg); }

	
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
	colored = tlucanti::replace(colored, "[K]", tlucanti::color::k);
	colored = tlucanti::replace(colored, "[R]", tlucanti::color::r);
	colored = tlucanti::replace(colored, "[G]", tlucanti::color::g);
	colored = tlucanti::replace(colored, "[Y]", tlucanti::color::y);
	colored = tlucanti::replace(colored, "[B]", tlucanti::color::b);
	colored = tlucanti::replace(colored, "[P]", tlucanti::color::p);
	colored = tlucanti::replace(colored, "[C]", tlucanti::color::c);
	colored = tlucanti::replace(colored, "[W]", tlucanti::color::w);
	colored = tlucanti::replace(colored, "[]", tlucanti::color::s);
	std::cout << colored;
	return out;
}

const tlucanti::color &operator <<(const tlucanti::color &out, const std::exception &exc)
{
	std::string msg = exc.what();
	return out << msg;
}

const tlucanti::color &operator <<(const tlucanti::color &out, const char *msg)
{
	return out << std::string(msg);
}

}
