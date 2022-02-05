/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:38:49 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 14:26:00 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_HPP
# define DATABASE_HPP

# include <string>
# include <vector>

namespace tlucanti
{
	class Database
	{
	public:

	private:
		typedef	std::list<User>							user_container_type;
		typedef	std::unordered_map<int, User &>			fd_hashmap_type;
		typedef	std::unordered_map<std::string, User &>	string_hashmap_type;

		typedef std::unordered_map<std::string, Channel &>
			channel_container_type;

		user_container_type		users;
		fd_hashmap_type			fd_access;
		string_hashmap_type		string_access;
		channel_list_type		cahnnels;
	};
}

#endif /* DATABASE_HPP */
