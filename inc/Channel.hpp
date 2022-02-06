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
# if __cpluplus <= 199711L
#  include <set>
# else
#  include <unordered_set>
# endif

# include "User.hpp"
# include "defs.h"

namespace tlucanti
{
	struct Channel
	{
		Channel(const std::string &name, const User &creator);

#if __cpluplus <= 199711L
		typedef std::set<int> user_container_type;
		typedef std::set<int> oper_conrainer_type;
#else
		typedef std::unordered_set<int>	user_container_type;
		typedef std::unordered_set<int>	oper_conrainer_type;
#endif

		inline void add_user(const User &new_user);
		inline void add_oper(const User &new_oper);
		inline void remove_user(const User &del_user);
		inline void remove_oper(const User &del_oper);

		void send_message(const std::string &message) const;

		__WUR inline const std::string &get_name() const { return name; }
		__WUR inline const std::string &get_topic() const { return _topic; }
		inline void new_topic(const std::string &topic);

		__WUR inline bool operator ==(const Channel &cmp) const { return is_nil && cmp.is_nil; }

		static const Channel nil;
		explicit Channel(bool) noexcept : is_nil(true) {}

	private:
		std::string			name;
		std::string			_topic;
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
