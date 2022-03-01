/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:29:25 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/14 10:31:20 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <set>

# include "ITarget.hpp"

# include "defs.h"
# include "Exception.hpp"
# include "IRCParserException.hpp"
# include "global.h"

namespace tlucanti
{
	struct Channel : public ITarget
	{
		Channel(const std::string &name);
		~Channel() override __DEFAULT

		typedef std::list<ITarget *> user_container_type;
		typedef std::set<ITarget *> special_conrainer_type;

		void add_user(const ITarget &new_user);
		void remove_user(const ITarget &del_user);
		void add_oper(const ITarget &new_oper);
		void remove_oper(const ITarget &del_oper);
		void add_voice(const ITarget &new_voice);
		void remove_voice(const ITarget &del_voive);
		void add_ban(const ITarget &new_ban);
		void remove_ban(const ITarget &del_ban);

		__WUR bool has_mode(const std::string &mode) const override;
		void make_mode(const std::string &mode) override;
		__WUR std::string get_modes() const override;
		__WUR bool check_pass(const std::string &pass) const; // ok - true, not ok - false, if server has no password - always return true
		__WUR bool is_oper(const ITarget &target) const;
		__WUR bool is_voice(const ITarget &target) const;
		__WUR bool is_banned(const ITarget &target) const;

		void send_message(const std::string &message) const override;
		void send_message(const std::string &message, const ITarget &except) const;
		void make_topic(const std::string &_topic, const ITarget &author);
		void make_pass(const std::string &pass);
		inline void make_limit(unsigned int n) { max_users = n; }

		__WUR const std::string &get_name() const override { return name; }
		__WUR inline const std::string &get_topic() const { return topic; }
		__WUR inline const user_container_type &get_users() const { return users; }
		__WUR inline const std::string &get_topic_author() const { return topic_author; }
		__WUR inline time_t get_topic_time() const { return topic_time; }
		__WUR inline time_t get_create_time() const { return create_time; }

		__WUR ITarget *operator [](const ITarget &user) const;
		__WUR ITarget *operator [](const std::string &user) const;

		static const char *modes;

	private:
		std::string			name;
		std::string			topic;
		std::string			pass;
		std::string 		topic_author;
		time_t				topic_time;
		time_t 				create_time;
		unsigned int		max_users;

		struct Modes
		{
			bool 	limit;	// (l) mode (max users limit)
			bool 	inv;	// (i) mode (invite only channel)
			bool 	key;	// (k) mode (requires password)
			bool 	moder;	// (m) mode (moderated voice)
			bool 	secret;	// (s) mode (secret channel)
			bool	topic;	// (t) mode (topic protected)
			bool 	next;	// (n) mode (no external messages)
		} _modes;

		user_container_type		users;
		special_conrainer_type	operators;
		special_conrainer_type	voices;
		special_conrainer_type	banned;

	__DELETED_MEMBERS:
		Channel() __DELETE
		Channel(const Channel &) __DELETE
		Channel &operator =(const Channel &) __DELETE
	};

	std::ostream &operator <<(std::ostream &out, const Channel &chan);
}

#endif /* CHANNEL_HPP */
