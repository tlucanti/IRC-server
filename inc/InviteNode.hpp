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
		InviteNode(const User *user, const Channel *channel, time_t expire)
			: user(user), channel(channel), expire(expire) {}
		~InviteNode() __DEFAULT
		const User *user;
		const Channel *channel;
		time_t expire;

		__WUR inline bool operator ==(const InviteNode &cmp) const
		{ return user == cmp.user and channel == cmp.channel; }
		__WUR inline bool operator <(const InviteNode &cmp) const
		{ return expire < cmp.expire; }

	__DELETED_MEMBERS:
#warning "__DELETE methods"
		InviteNode() __DEFAULT
		InviteNode(const InviteNode &) __DEFAULT
			InviteNode &operator =(const InviteNode &) __DEFAULT
	};
}

#endif /* INVITE_NODE_HPP */
