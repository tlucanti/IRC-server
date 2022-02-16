/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 22:02:03 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/16 22:19:43 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
# define GLOBAL_H

namespace tlucanti
{
	extern sig_atomic_t server_run;

	extern const char *server_name;
	extern const char *server_address;
	extern const char *server_version;
	extern const char *server_oper_login;
	extern const char *server_oper_password;

	extern std::string server_password;
	extern std::string server_begining;
	extern unsigned short server_port;

	extern const unsigned int channel_max_users;
	extern const unsigned int chanel_max_name_len;
	extern const unsigned int channel_max_topic_len;
	extern const unsigned int user_max_channels;
	extern const unsigned int user_max_nick_len;

	namespace IRC
	{
		extern const std::string endl;
	}

	template<class T, class U>
	struct is_same : std::false_type {};

	template<class T>
	struct is_same<T, T> : std::true_type {};

	template <typename T, typename Who>
	__WUR
	bool
	isinstance(__UNUSED const Who &)
	{
		return is_same<T, Who>::value;
	}
}

#endif /* GLOBAL_H */