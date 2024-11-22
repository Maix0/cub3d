/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_find_hitdirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 19:21:14 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 19:21:32 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "app/textures.h"
#include "me/vec2/vec2.h"
#include <math.h>

void	ray_set_tex(t_ray *ray, t_texture tex, t_f64 pos)
{
	ray->tex = tex;
	ray->percent_wall = pos;
}

void	hit_x_y(t_ray *ray)
{
	t_vf2d	dummy_int;
	t_vf2d	pos;
	t_vf2d	dist;

	ray->tex = TEX_NORTH;
	pos = vf2d(modf(ray->x, &dummy_int.x), modf(ray->y, &dummy_int.y));
	dist = vf2d(fmin(pos.x, 1 - pos.x), fmin(pos.y, 1 - pos.y));
	if (dist.x >= dist.y)
	{
		if (ray->direction >= 0)
			ray_set_tex(ray, TEX_SOUTH, 1 - pos.x);
		else
			ray_set_tex(ray, TEX_NORTH, pos.x);
	}
	else
	{
		if (ray->direction >= PI / 2 || ray->direction <= -PI / 2)
			ray_set_tex(ray, TEX_WEST, 1 - pos.y);
		else
			ray_set_tex(ray, TEX_EAST, pos.y);
	}
}
