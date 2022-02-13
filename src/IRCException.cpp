/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 14:00:46 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:45 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRCException.hpp"

tlucanti::IRCException::IRCException(const char *func, int line, const char *message, const std::string &arg)
{
	std::stringstream ss;
	_message = func;
	_message.erase(std::find(_message.begin(), _message.end(), '('), _message.end());
	std::string::iterator it = --_message.end();
	while (isalnum(*it) or *it == ':' or *it == '_')
		--it;
	_message.erase(_message.begin(), ++it);
	ss << "[r][FAIL][w] [" << _message << "::" << line << "][] " << message <<
		": [y]" << arg << "[]";
	_message = ss.str();
}

tlucanti::IRCException::IRCException(const std::string &parent,
	const std::string &message, int error)
		: _parent(parent)
{
	_message = message;
	_message = "[r][FAIL][w] " + _parent + ":[] " + _message + ": [y]" +
		strerror(error) + "[]";
}

tlucanti::IRCException::IRCException(const std::string &parent,
	const std::string &message)
		: _parent(parent)
{
	_message = message;
	_message = "[r][FAIL][w] " + _parent + ":[] " + _message;
}

tlucanti::IRCException::IRCException(const std::string &parnet,
	const std::string &message, const std::string &error)
		: _parent(parnet)
{
	_message = message;
	_message = "[r][FAIL][w] " + _parent + ":[] " + _message + ": [y]" +
		error + "[]";
}

const char *
tlucanti::IRCException::what() const noexcept
{
	return _message.c_str();
}
