/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f64.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:04:25 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/05 14:06:52 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef F64_H
#define F64_H

#include "me/types.h"


/// @brief clamp a f64 to be between a minimun and maximum (inclusive)
/// @param min the minimun bound
/// @param value the value to clamp
/// @param max value the maximum
/// @note if any value is NaN, NaN is returned
t_f64 f64_clamp(t_f64 min, t_f64 value, t_f64 max);

#endif /* F64_H */
