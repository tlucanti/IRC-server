/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 17:49:10 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/08 18:31:26 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
# include <list>
# include <unordered_set>

# include "defs.h"
# include "IRCParserException.hpp"
# include "Socket.hpp"
# include "IRCcodes.h"
# include "IRCrpl.hpp"
# include "Channel.hpp"

namespace tlucanti
{
	struct Channel;

	struct User
	{
	public:
		User(int sock) noexcept;
		~User() __DEFAULT

		__WUR inline int get_sock() const { return sock_fd; }

		void send_message(const std::string &message) const;
		void send_to_chanels(const std::string &message) const;

		void add_channel(Channel &channel);
		void remove_channel(Channel &channel);

		void assert_mode(const std::string &mode) const;
		__WUR bool has_mode(const std::string &mode) const;
		void make_mode(const std::string &mode);

		void make_nickname(const std::string &nickname);
		void make_user(const std::string &nickname, const std::string &hostname,
			const std::string &servername, const std::string &realname);

		__WUR inline const std::string &get_nickname() const { return _nickname; }
		__WUR std::string compose() const;

		static const char *modes;
	private:
		typedef std::unordered_set<Channel *> channels_list;

		int			sock_fd;		// new socked appeared
		struct Modes
		{
			bool		pass;		// PASS command recived
			bool		nick;		// NICK command recived
			bool		reg;		// USER + NICK + PASS received
			bool		oper;
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
