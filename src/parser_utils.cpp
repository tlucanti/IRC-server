/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 23:40:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 23:48:32 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

namespace tlucanti
{
	void split(const std::string &str, std::vector<std::string> out, char c)
	{
		std::stringstream ss(str);

		while (ss)
		{
			std::string next;
			std::getline(ss, next, c);
			out.push_back(next);
		}
	}

	std::string replace(const std::string &str, const std::string &from, const std::string &to)
	{
		if (from == to)
			return str;
		size_t it = str.find(from, 0);
		std::string ret = str;
		while (it != std::string::npos)
		{
			ret = ret.replace(it, from.size(), to);
			it += to.size();
			it = str.find(from);
		}
		return ret;
	}
}
