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

# include "defs.h"
# include "IRCParserException.hpp"
# include "Socket.hpp"
# include "IRCcodes.h"
# include "IRCrpl.hpp"

namespace tlucanti
{
	extern std::string server_address;
	extern std::string server_version;
}

namespace tlucanti
{
#define assert_mode(...) assert_mode__macro( \
	({ \
		const char *_fmt[] = {__VA_ARGS__}; \
		std::vector<std::string> _vec(_fmt, _fmt + \
			sizeof(_fmt) / sizeof(_fmt[0])); \
		_vec; \
	}))

#define has_mode(...) has_mode__macro( \
	({ \
		const char *_fmt[] = {__VA_ARGS__}; \
		std::vector<std::string> _vec(_fmt, _fmt + \
			sizeof(_fmt) / sizeof(_fmt[0])); \
		_vec; \
	}))

#define make_mode(...) make_mode__macro( \
	({ \
		const char *_fmt[] = {__VA_ARGS__}; \
		std::vector<std::string> _vec(_fmt, _fmt + \
			sizeof(_fmt) / sizeof(_fmt[0])); \
		_vec; \
	}))

	struct User
	{
		User(int sock) noexcept;
		~User() __DEFAULT

		__WUR inline int get_sock() const { return sock_fd; }

		void send_message(const std::string &message) const;

		void assert_mode__macro(const std::vector<std::string> &format) const;
		__WUR bool has_mode__macro(const std::vector<std::string> &format) const;
		void make_mode__macro(const std::vector<std::string> &format);

		void make_nickname(const std::string &nickname);
		void make_user(const std::string &nickname, const std::string &hostname,
			const std::string &servername, const std::string &realname);

		__WUR inline const std::string &get_nickname() const { return _nickname; }
		__WUR inline const std::string &get_username() const { return _username; }
		__WUR inline const std::string &get_hostname() const { return _hostname; }

		__WUR std::string compose() const;

		static const char *modes;
	private:

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

	__DELETED_MEMBERS:
		User() __DELETE
		User(const User &user) __DELETE
	};
	std::ostream &operator <<(std::ostream &out, const tlucanti::User &usr);
}


#endif /* USER_HPP */
