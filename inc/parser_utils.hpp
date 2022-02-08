/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 10:56:57 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/07 20:58:59 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_UTILS_HPP
# define PARSER_UTILS_HPP

# include <vector>
# include <string>
# include <sstream>
# include <iterator>

namespace tlucanti::IRC
{
	extern std::string endl;
}

namespace tlucanti
{
	template<class T, class U>
	struct is_same : std::false_type {};

	template<class T>
	struct is_same<T, T> : std::true_type {};

	template <typename T, typename Who>
	bool isinstance(__UNUSED const Who &)
	{
		return is_same<T, Who>::value;
	}

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

//	inline std::string compose_message(const User &from,
//		irc_code_t code, const User &recepient, const std::string &message)
//	{
//		std::string _from;
//		if (from == nullptr)
//			_from = tlucanti::server_name;
//		else
//			_from = from.compose();
//		std::string ret = ':' + _from + ' ' +
//			std::to_string(code) + ' ' + recepient.get_nickname() +
//			+ " :" + message + IRC::endl;
//		return ret;
//	}

//	inline std::string compose_message(const User &from,
//		const std::string &command, const std::string &recepient, const std::string &message)
//	{
//		std::string _from;
//		if (from == User::nil)
//			_from = tlucanti::server_name;
//		else
//			_from = from.compose();
//		std::string ret = ':' + _from + ' ' + command + ' ' + recepient + " :" + message + IRC::endl;
//		return ret;
//	}

//	inline std::string compose_message(const User &from,
//		const std::string &command, const User &recepient, const std::string &message)
//	{
//		return compose_message(from, command, recepient.get_nickname(), message);
//	}

//	inline std::string compose_message(const User &from,
//									   irc_code_t code, const std::string &message)
//	{
//		std::string _from;
//		if (from == User::nil)
//			_from = tlucanti::server_name;
//		else
//			_from = from.compose();
//		std::string ret = ':' + _from + ' ' +
//						  std::to_string(code) + ' ' +
//						  message + IRC::endl;
//		return ret;
//	}

	inline std::string get_current_time() {
		const char *month[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September",
							   "October", "November", "December"};
		const char *day[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
		char buf[100] = {};
		std::string ret;

		time_t now = time(nullptr);
		struct tm tstruct = *localtime(&now);
		bzero(buf, sizeof(buf));
		strftime(buf, sizeof(buf) / sizeof(char), "%d", &tstruct);
		ret += day[atoi(buf)];
		bzero(buf, sizeof(buf));
		strftime(buf, sizeof(buf) / sizeof(char), "%m", &tstruct);
		ret += ' ';
		ret += month[atoi(buf)];
		bzero(buf, sizeof(buf));
		strftime(buf, sizeof(buf) / sizeof(char), "%d", &tstruct);
		ret += ' ';
		ret += buf;
		bzero(buf, sizeof(buf));
		strftime(buf, sizeof(buf) / sizeof(char), "%Y", &tstruct);
		ret += ' ';
		ret += buf;
		ret += " at ";
		bzero(buf, sizeof(buf));
		strftime(buf, sizeof(buf) / sizeof(char), "%X", &tstruct);
		ret += buf;
		return ret;
	}
}

#endif /* PARSER_UTILS_HPP */