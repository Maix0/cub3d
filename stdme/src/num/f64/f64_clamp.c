/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f64_clamp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:03:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:16:29 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/num/f64.h"

t_f64	f64_clamp(t_f64 min, t_f64 value, t_f64 max)
{
	if (min != min || value != value || max != max)
		return (0. / 0.);
	if (min > value)
		return (min);
	if (value > max)
		return (max);
	return (value);
}
