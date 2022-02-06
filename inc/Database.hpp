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
	class Database
	{
	public:
		Database() __DEFAULT
		~Database();

		void add_client(const Socket &sock);
		void remove_client(Socket &sock);
		void remove_client(const std::string &nickname);
		__WUR bool make_edge(const std::string &nickname, const Socket &sock);

		__WUR User &operator[](int fd) const;
		__WUR User &operator[](const std::string &nickname) const;
		__WUR User &operator[](const Socket &client) const;
		__WUR Channel &get_channel(const std::string &channel) const;
	private:
		typedef	std::unordered_map<int, User *>			fd_hashmap_type;
		typedef	std::unordered_map<std::string, User *>	string_hashmap_type;
		typedef std::unordered_map<std::string, Channel *>
				channel_container_type;

		fd_hashmap_type			fd_access;
		string_hashmap_type		str_access;
		channel_container_type	cahnnels;

	__DELETED_MEMBERS:
		Database(const Database &) __DELETE
		Database &operator =(const Database &) __DELETE
	};
}

#endif /* DATABASE_HPP */
