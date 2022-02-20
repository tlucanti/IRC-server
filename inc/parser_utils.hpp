/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 10:56:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/19 13:56:38 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_HPP
# define PARSER_UTILS_HPP

# include <vector>
# include <string>
# include <sstream>
# include <iterator>
# include <set>

# include "global.h"

namespace tlucanti
{
	inline
	void
	split(const std::string &str, std::vector<std::string> &out, char c=' ')
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

	__WUR inline
	std::string
	squeeze(const std::string &str)
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

	__WUR inline
	std::string
	replace(const std::string &str, const std::string &from, const std::string &to)
	{
		if (from == to)
			return str;
		std::string ret = str;
		size_t it = ret.find(from, 0);
		while (it != std::string::npos)
		{
			ret = ret.replace(it, from.size(), to);
			it += to.size();
			it = ret.find(from, it);
		}
		return ret;
	}

	__WUR inline
	std::string
	strip(const std::string &str, char c=' ')
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

	__WUR inline
	std::string
	truncate(const std::string &str, size_t maxlen=10)
	{
		if (str.size() <= maxlen)
			return str;
		else
			return str.substr(0, maxlen - 2) + "..";
	}

	__WUR inline
	bool
	contains_only(const std::string &str, const std::string &format)
	{
		std::set<char> str_chars(str.begin(), str.end());
		std::set<char> format_chars(format.begin(), format.end());
		std::set<char> union_chars;
		std::set_union(str_chars.begin(), str_chars.end(), format_chars.begin(),
			format_chars.end(), std::inserter(union_chars, union_chars.begin()));
		if (union_chars.size() > format_chars.size())
			return false;
		return true;
	}
}

#endif /* PARSER_UTILS_HPP */
