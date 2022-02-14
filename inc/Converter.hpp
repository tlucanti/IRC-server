/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Converter.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 13:02:00 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/14 13:20:03 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONVERTER_HPP
# define CONVERTER_HPP

# include <string>
# include <stdexcept>
# include <sstream>

# include "parser_utils.hpp"

namespace tlucanti
{
	template <typename value_T>
	class Converter
	{
	public:
		typedef value_T value_type;

		explicit Converter(const value_type &val)
			: _val(val) {}

		explicit operator int() const
		{
			if (is_same<value_T, std::string>::value)
			{
				std::stringstream ss(strip(_val));
				int n;
				ss >> n;
				if (not ss.eof() or ss.fail())
					throw std::invalid_argument("invalid value");
				return n;
			}
			else
				throw std::invalid_argument("type is not supported");
		}

	private:
		const value_type &_val;
	};
}

#endif /* CONVERTER_HPP */
