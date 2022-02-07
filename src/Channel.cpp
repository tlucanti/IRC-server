/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:21:17 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:37 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

const tlucanti::Channel tlucanti::Channel::nil = tlucanti::Channel(true);
const char *Channel::modes = "opsitnmlvk";

tlucanti::Channel::Channel(const std::string &name, const User &creator)
		: name(name), is_nil(false)
{
	add_user(creator);
	add_oper(creator);
}

void
tlucanti::Channel::add_user(const User &new_user)
{
	users.insert(new_user.get_sock());
}

void
tlucanti::Channel::add_oper(const User &new_oper)
{
	operators.insert(new_oper.get_sock());
}

void
tlucanti::Channel::remove_user(const User &del_user)
{
	users.erase(del_user.get_sock());
}

void
tlucanti::Channel::remove_oper(const User &del_oper)
{
	users.erase(del_oper.get_sock());
}

void
tlucanti::Channel::new_topic(const std::string &topic)
{
	_topic = topic;
}

void
tlucanti::Channel::send_message(const std::string &message) const
{
	for (user_container_type::iterator it=users.begin(); it != users.end(); ++it)
	{
		Socket(*it).send(message);
	}
}
