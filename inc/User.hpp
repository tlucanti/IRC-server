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
# include <unordered_set>

# include "ITarget.hpp"

# include "defs.h"
# include "IRCParserException.hpp"
# include "Socket.hpp"
# include "IRCcodes.h"
# include "IRCrpl.hpp"
# include "Channel.hpp"

namespace tlucanti
{
	struct Channel;

	struct User : public ITarget
	{
	public:
		User(int sock) noexcept;
		~User() override __DEFAULT

		__WUR inline int get_sock() const { return sock_fd; }

		void send_message(const std::string &message) const override;
		void send_to_chanels(const std::string &message) const;

		void add_channel(Channel &channel);
		void remove_channel(Channel &channel);

		void assert_mode(const std::string &mode) const override;
		__WUR bool has_mode(const std::string &mode) const override;
		void make_mode(const std::string &mode) override;
		__WUR std::string get_modes() const override;

		void make_nickname(const std::string &nickname);
		void make_user(const std::string &nickname, const std::string &hostname,
			const std::string &servername, const std::string &realname);

		__WUR const std::string &get_name() const override { return _nickname; }
		__WUR std::string compose() const;

		static const char *modes;
	private:
		typedef std::unordered_set<Channel *> channels_list;

		int			sock_fd;		// new socked appeared
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
		std::string	_hostname;
		std::string _servername;
		std::string _realname;

		channels_list channels_member;

	__DELETED_MEMBERS:
		User() __DELETE
		User(const User &user) __DELETE
	};

	std::ostream &operator <<(std::ostream &out, const tlucanti::User &usr);
}

#endif /* USER_HPP */
