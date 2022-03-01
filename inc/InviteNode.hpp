/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteNode.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 13:40:05 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/16 13:47:01 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_NODE_HPP
# define INVITE_NODE_HPP

# include "defs.h"
# include "User.hpp"
# include "Channel.hpp" 

namespace tlucanti
{
	struct InviteNode
	{
		InviteNode(const std::string &user, const std::string &channel, time_t expire)
				: user(user), channel(channel), expire(expire) {}
		InviteNode(const InviteNode &cpy)
				: user(cpy.user), channel(cpy.channel), expire(cpy.expire) {}
		~InviteNode() __DEFAULT
		const std::string &user;
		const std::string &channel;
		time_t expire;

		__WUR inline bool operator ==(const InviteNode &cmp) const
		{ return user == cmp.user and channel == cmp.channel; }
		__WUR inline bool operator <(const InviteNode &cmp) const
		{ return expire < cmp.expire; }

	__DELETED_MEMBERS:
		InviteNode() __DELETE
		InviteNode &operator =(const InviteNode &) __DELETE
	};
}

#endif /* INVITE_NODE_HPP */
