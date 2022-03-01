/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ITarget.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 18:55:53 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/11 10:25:35 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITARGET_HPP
# define ITARGET_HPP

# include <string>
# include <ostream>

# include "defs.h"

namespace tlucanti
{
	class ITarget
	{
	public:
		virtual void send_message(const std::string &message) const = 0;
		__WUR virtual bool has_mode(const std::string &mode) const = 0;
		virtual void make_mode(const std::string &mode) = 0;
		__WUR virtual const std::string &get_name() const = 0;
		__WUR virtual std::string get_modes() const = 0;

	 protected:
		ITarget() __DEFAULT
		virtual ~ITarget() __DEFAULT

	__DELETED_MEMBERS:
		ITarget(const ITarget &) __DELETE
		ITarget &operator =(const ITarget &) __DELETE
	};

	inline std::ostream &operator <<(std::ostream &out, const tlucanti::ITarget &usr)
	{
		out << usr.get_name();
		return out;
	}
}

#endif /* ITARGET_HPP */
