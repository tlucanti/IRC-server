/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 17:49:10 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 13:23:47 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>

namespace tlucanti
{
	struct User
	{
		int			sock_fd;		// new socked appeared
		bool		authorized;		// PASS command recived
		bool		authenticated;	// USER command recived
		bool		irc_operator;

		std::string	nickname;
		std::string	hostname;
		std::string servername;
		std::string realname;
	};
}

#endif /* USER_HPP */
