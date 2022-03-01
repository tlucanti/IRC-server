/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 14:00:46 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/28 17:00:43 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Exception.hpp"

tlucanti::Exception::Exception(const char *func, int line, const char *message, const std::string &arg)
{
	std::stringstream ss;
	_message = func;
	_message.erase(std::find(_message.begin(), _message.end(), '('), _message.end());
	std::string::iterator it = --_message.end();
	while (isalnum(*it) or *it == ':' or *it == '_')
		--it;
	_message.erase(_message.begin(), ++it);
	ss << "[r][FAIL][w] [" << _message << "::" << line << "][] " << message <<
		": [y]" << arg << ' ' << total_errors() << "[]";
	_message = ss.str();
}

tlucanti::Exception::Exception(const std::string &parent,
	const std::string &message, int error)
{
	_message = message;
	_message = "[r][FAIL][w] " + parent + ":[] " + _message + ": [y]" +
		strerror(error) + ' ' + total_errors() + "[]";
}

tlucanti::Exception::Exception(const std::string &parent,
	const std::string &message)
{
	_message = message;
	_message = "[r][FAIL][w] " + parent + ":[] " + _message + ' ' + total_errors() + "[]";
}

tlucanti::Exception::Exception(const std::string &parent,
	const std::string &message, const std::string &error)
{
	_message = message;
	_message = "[r][FAIL][w] " + parent + ":[] " + _message + ": [y]" +
		error + ' ' + total_errors() + "[]";
}

const char *
tlucanti::Exception::what() const noexcept
{
	return _message.c_str();
}

std::string
tlucanti::Exception::total_errors() const
{
	static int total_errors = 0;

	std::stringstream ss;
	ss << "[p](total errors: " << ++total_errors << ")[]";
	return ss.str();
}

