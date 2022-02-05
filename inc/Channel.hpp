/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 13:21:11 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/05 13:32:18 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

namespace tlucanti
{
	struct Channel
	{
		typedef std::list<int>	user_container_type;
		typedef std::list<int>	oper_conrainer_type

		std::string			name;
		std::string			topic;
		bool				is_local; // & if true, # if false

		user_container_type	users;
		oper_conrainer_type	operators;
	};
}

#endif /* CHANNEL_HPP */
