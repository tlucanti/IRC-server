/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 10:56:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 22:50:37 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#include "User.hpp"

namespace tlucanti
{
	inline void split(const std::string &str, std::vector<std::string> &out, char c=' ')
	{
		if (c == ' ')
		{
			std::istringstream ss(str);
			while (true)
			{
				std::string next;
				ss >> next;
				if (next.empty())
					return ;
				out.push_back(next);
			}
		}
		else
		{
			std::stringstream ss(str);

			while (ss)
			{
				std::string next;
				std::getline(ss, next, c);
				out.push_back(next);
			}
			out.pop_back();
		}
	}

	inline std::string squeeze(const std::string &str)
	{
		std::vector<std::string> _split;
		split(str, _split);
		std::string squeeze_str;
		if (_split.empty())
			return squeeze_str;
		squeeze_str = _split.at(0);
		for (std::vector<std::string>::iterator it=++_split.begin(); it != _split.end(); ++it)
			squeeze_str += ' ' + *it;
		return squeeze_str;
	}

	inline std::string replace(const std::string &str, const std::string &from, const std::string &to)
	{
		if (from == to)
			return str;
		size_t it = str.find(from, 0);
		std::string ret = str;
		while (it != std::string::npos)
		{
			ret = ret.replace(it, from.size(), to);
			it += to.size();
			it = str.find(from, it);
		}
		return ret;
	}

	inline std::string strip(const std::string &str, char c=' ')
	{
		std::string ret = str;
		if (c == ' ')
		{
			{
				std::string::iterator it = ret.begin();
				for (; it != ret.end(); ++it) {
					if (not std::isspace(*it))
						break ;
				}
				ret.erase(ret.begin(), it);
			}
			{
				int it = (int)ret.size() - 1;
				for (; it > 0; --it) {
					if (not std::isspace(ret.at(it))) {
						break ;
					}
				}
				ret.erase(it + 1);
			}
			ret.shrink_to_fit();
			return ret;
		}
		else
		{
			{
				std::string::iterator it = ret.begin();
				for (; it != ret.end(); ++it) {
					if (*it != c)
						break ;
				}
				ret.erase(ret.begin(), it);
			}
			{
				std::string::reverse_iterator it = ret.rbegin();
				for (; it != ret.rend(); ++it) {
					if (*it != c) {
						break ;
					}
				}
			}
			return ret;
		}
	}

	inline std::string compose_message(const User &from,
		irc_code_t code, std::string &message)
	{
		std::string ret = ':' + tlucanti::server_name +
			from.get_nickname() + ':' + message;
		return ret;
	}
}
