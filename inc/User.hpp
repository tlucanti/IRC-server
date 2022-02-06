/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 17:49:10 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:35 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>

# include "defs.h"
# include "IRCParserException.hpp"
# include "Socket.hpp"
# include "IRCcodes.h"

namespace tlucanti
{
	#define check_perm(...) _check_perm__macro( \
	({ \
		const char *_fmt[] = {__VA_ARGS__}; \
		std::vector<std::string> _vec(_fmt, _fmt + sizeof(_fmt) / sizeof(_fmt[0])); \
		_vec; \
	}))

	struct User
	{
		User(int sock) noexcept;
		~User() __DEFAULT

		__WUR inline int get_sock() const { return sock_fd; }
		__WUR inline bool has_pass() const { return _has_pass; }
		__WUR inline bool has_nick() const { return _has_nick; }
		__WUR inline bool has_user() const { return _has_user; }
		__WUR inline bool irc_operator() const { return _irc_operator; }

		void send_message(const std::string &message) const;

		inline void make_pass() { _has_pass = true; }
		void make_user(const std::string &nickname, const std::string &hostname,
			const std::string &servername, const std::string &realname);
		inline void make_irc_operator() { _irc_operator = true; }
		void make_nickname(const std::string &nickname);

		__WUR inline const std::string &get_nickname() const { return _nickname; }
		__WUR inline const std::string &get_hostname() const { return _hostname; }

		__WUR inline bool operator ==(const User &cmp) const { return sock_fd == cmp.sock_fd; }

		void _check_perm__macro(const std::vector<std::string> &format) const;

		static const User nil;

	private:
		int			sock_fd;		// new socked appeared
		bool		_has_pass;		// PASS command recived
		bool		_has_nick;		// NICK command recived
		bool		_has_user;		// USER command recived
		bool		_irc_operator;

		std::string	_nickname;
		std::string	_hostname;
		std::string _servername;
		std::string _realname;

	__DELETED_MEMBERS:
		User() __DELETE
		User(const User &user) __DELETE
		User &operator =(const User &user) __DELETE
	};
}

#endif /* USER_HPP */
