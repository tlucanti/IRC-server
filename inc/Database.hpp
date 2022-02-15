/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:36:58 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:13 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_HPP
# define DATABASE_HPP

# include <string>
# include <vector>
# include <list>
# include <unordered_map>

# include "User.hpp"
# include "Channel.hpp"
# include "Socket.hpp"

namespace tlucanti
{
	class User;
	class Channel;
	class Database
	{
	public:
		typedef std::unordered_map<std::string, Channel *>
				channel_container_type;

		Database() noexcept : invisible_users(0), operators_cnt(0) {}
		~Database();

		Channel *add_channel(const std::string &name);
		void add_client(const Socket &sock);
		void remove_client(User &user);
		__WUR bool make_edge(const std::string &nickname, const Socket &sock);

		__WUR User *operator[](const std::string &nickname) const;
		__WUR User *operator[](const Socket &client) const;
		__WUR Channel *get_channel(const std::string &channel) const;

		__WUR inline int get_user_cnt() const { return (int)sock_access.size(); }
		__WUR inline int get_chan_cnt() const { return (int)channels.size(); }
		__WUR inline const channel_container_type &get_channels() const { return channels; }

		int invisible_users;
		int operators_cnt;
		const int max_users = 500;
	private:
		typedef	std::unordered_map<int, User *>			sock_hashmap_type;

		typedef	std::unordered_map<std::string, User *>		string_hashmap_type;

		sock_hashmap_type		sock_access;
		string_hashmap_type		str_access;
		channel_container_type	channels;

	__DELETED_MEMBERS:
		Database(const Database &) __DELETE
		Database &operator =(const Database &) __DELETE
	};
}


#endif /* DATABASE_HPP */
