/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 16:58:26 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/06 17:43:10 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
# define COLOR_HPP

# include <iostream>
# include <string>

# include "parser_utils.hpp"
# include "defs.h"

namespace tlucanti
{
	class color
	{
	public:
		static const char *k; // black
		static const char *r; // red
		static const char *g; // green
		static const char *y; // yellow
		static const char *b; // blue
		static const char *p; // purple
		static const char *c; // cyan
		static const char *w; // white
		static const char *s; // reset

		static const char *tlucanti;

		color() __DEFAULT

	__DELETED_MEMBERS:
		color(const color &) __DELETE
		color &operator =(const color &) __DELETE
	};

	const color cout;
	extern const char *endl;
}

namespace tlucanti
{
	void WARN(const std::string &parent, int error, const std::string &msg);
	void WARN(const std::string &parent, const std::string &msg);
	void ERROR(const std::string &parent, int error, const std::string &msg);
	void ERROR(const std::string &parent, const std::string &msg);
	void INFO(const std::string &parent, int error, const std::string &msg);
	void INFO(const std::string &parent, const std::string &msg);
//}

const tlucanti::color &operator <<(const tlucanti::color &out, const std::string &message);
const tlucanti::color &operator <<(const tlucanti::color &out, const std::exception &exc);
const tlucanti::color &operator <<(const tlucanti::color &out, const char *msg);

}
#endif  // COLOR_HPP
