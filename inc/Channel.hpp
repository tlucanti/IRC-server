/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:29:25 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:08 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <set>

# include "defs.h"
# include "User.hpp"
# include "IRCParserException.hpp"

namespace tlucanti
{
	struct User;

	struct Channel
	{
		Channel(const std::string &name);

		typedef std::set<int> user_container_type;
		typedef std::set<int> oper_conrainer_type;

		inline void add_user(const User &new_user);
		inline void add_oper(const User &new_oper);
		inline void remove_user(const User &del_user);
		inline void remove_oper(const User &del_oper);

		void assert_mode(const std::string &mode) const;
		bool has_mode(const std::string &mode) const;
		void make_pass(const std::string &pass);
		bool check_pass(const std::string &pass) const; // ok - true, not ok - false, if server has no password - always return true

		void send_message(const std::string &message) const;

		__WUR inline const std::string &get_name() const { return name; }
		__WUR inline const std::string &get_topic() const { return topic; }
		__WUR inline const user_container_type &get_users() const { return users; }
		inline void new_topic(const std::string &topic);

		static const char *modes;

	private:
		std::string			name;
		std::string			topic;
		std::string			pass;
		bool is_nil;

		user_container_type	users;
		oper_conrainer_type	operators;

	__DELETED_MEMBERS:
		Channel() __DELETE
		Channel(const Channel &) __DELETE
		Channel &operator =(const Channel &) __DELETE
	};
}

#endif /* CHANNEL_HPP */
