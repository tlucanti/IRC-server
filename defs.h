/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 12:09:17 by kostya            #+#    #+#             */
/*   Updated: 2022/02/03 16:02:05 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
# define DEFS_H

# if __cplusplus <= 199711L
#  define __WUR __attribute__((warn_unused_result))
#  define noexcept throw()
#  define override
#  ifndef nullptr
#   define nullptr NULL
#  endif
#  define __CONST const
#  define __DEFAULT {}
#  define __DEFAULT_ASSIGN {return *this;}
#  define __DEFINE
#  define __OVERRIDE_VIRTUAL virtual
#  define __UNUSED __attribute__((unused))
#  define __EXPLICIT_OPERATOR

# else
#  define __WUR [[nodiscard]]
#  define __CONST
#  define __DEFAULT =default;
#  define __DEFAULT_ASSIGN __DEFAULT
#  define __DEFINE {};
#  define __OVERRIDE_VIRTUAL
#  define __UNUSED [[maybe_unused]]
#  define __EXPLICIT_OPERATOR explicit

# endif /* 199711L */
#endif /* DEFS_H */
