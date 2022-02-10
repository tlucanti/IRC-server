/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:29:25 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/10 20:34:22 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <set>

# include "ITarget.hpp"

# include "defs.h"
# include "IRCException.hpp"
# include "IRCParserException.hpp"

namespace tlucanti
{
	extern const unsigned int channel_max_users;

	struct Channel : public ITarget
	{
		Channel(const std::string &name);

		typedef std::list<ITarget *> user_container_type;
		typedef std::list<ITarget *> oper_conrainer_type;

		void add_user(const ITarget &new_user);
		void add_oper(const ITarget &new_oper);
		void remove_user(const ITarget &del_user);
		void remove_oper(const ITarget &del_oper);

		void assert_mode(const std::string &mode) const override;
		__WUR bool has_mode(const std::string &mode) const override;
		void make_mode(const std::string &mode) override;
		__WUR std::string get_modes() const override;
		void make_pass(const std::string &pass);
		__WUR bool check_pass(const std::string &pass) const; // ok - true, not ok - false, if server has no password - always return true

		void send_message(const std::string &message) const override;

		__WUR const std::string &get_name() const override { return name; }
		__WUR inline const std::string &get_topic() const { return topic; }
		__WUR inline const user_container_type &get_users() const { return users; }
		inline void new_topic(const std::string &topic);

		static const char *modes;

	private:
		std::string			name;
		std::string			topic;
		std::string			pass;

		struct Modes
		{
			bool 	key;	// (k) mode (requires password)
		} _modes;

		user_container_type	users;
		oper_conrainer_type	operators;

	__DELETED_MEMBERS:
		Channel() __DELETE
		Channel(const Channel &) __DELETE
		Channel &operator =(const Channel &) __DELETE
	};

	std::ostream &operator <<(std::ostream &out, const Channel &chan);
}

#endif /* CHANNEL_HPP */
