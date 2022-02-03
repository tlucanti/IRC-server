/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:33:50 by tlucanti          #+#    #+#             */
/*   Updated: 2022/02/03 16:02:04 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
# define COLOR_HPP

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
    };

    const char *color::k  = "\033[1;90m";
    const char *color::r  = "\033[1;91m";
    const char *color::g  = "\033[1;92m";
    const char *color::y  = "\033[1;93m";
    const char *color::b  = "\033[1;94m";
    const char *color::p  = "\033[1;95m";
    const char *color::c  = "\033[1;96m";
    const char *color::w  = "\033[1;97m";
    const char *color::s  = "\033[0m";

    const char *color::tlucanti   = (char *) "\033[19;1;96;21;6m";
}

#endif  // COLOR_HPP
