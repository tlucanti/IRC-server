/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 17:49:10 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/11 11:11:25 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
# include <list>
# include <set>

# include "ITarget.hpp"

# include "defs.h"
# include "IRCParserException.hpp"
# include "Socket.hpp"
# include "IRCcodes.h"
# include "IRCrpl.hpp"
# include "Channel.hpp"
# include "lexical_cast.hpp"

namespace tlucanti
{
	struct Channel;

	struct User : public ITarget
	{
	public:
		typedef std::set<Channel *> channels_list;

		explicit User(const Socket &sock) noexcept;
		~User() override __DEFAULT

		__WUR inline const Socket &get_sock() const { return sock; }

		void send_message(const std::string &message) const override;
		void send_to_channels(const std::string &message) const;

		void add_channel(Channel &channel);
		void remove_channel(Channel &channel);

		void assert_mode(const std::string &mode) const;
		__WUR bool has_mode(const std::string &mode) const override;
		void make_mode(const std::string &mode) override;
		__WUR std::string get_modes() const override;
		__WUR inline const channels_list &get_channels() const { return channels_member; }

		void make_nickname(const std::string &nickname);
		void make_user(const std::string &nickname, const std::string &realname);
		void complete_user();
		__WUR bool inline reg_waiting() const { return register_waiting; }
		void reset_ping();
		void do_ping();
		__WUR bool check_ping(const std::string &ping) const;

		__WUR const std::string &get_name() const override { return _nickname; }
		__WUR inline const std::string &get_username() const { return  _username; }
		__WUR inline const std::string &get_realname() const { return _realname; }
		__WUR std::string compose() const;

		static const char *modes;
	private:

		Socket			sock;
		struct Modes
		{
			bool		pass;		// PASS command received
			bool		nick;		// NICK command received
			bool		reg;		// (r) flag, USER + NICK + PASS received
			bool		oper;		// (o) operator flag
			bool		inv;		// (i) invisible flag
		} _modes;

		std::string	_nickname;
		std::string _username;
		std::string _realname;

		time_t last_ping;
		unsigned long long ping_message;
		bool ping_waiting;
		bool register_waiting;

		channels_list channels_member;

		friend void *ping_thread(void *);

	__DELETED_MEMBERS:
		User() __DELETE
		User(const User &user) __DELETE
	};

	std::ostream &operator <<(std::ostream &out, const tlucanti::User &usr);
}

#endif /* USER_HPP */
