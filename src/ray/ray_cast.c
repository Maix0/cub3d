/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 19:28:26 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 19:39:04 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/textures.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/mem/mem.h"
#include "me/vec2/vec2.h"
#include <math.h>

void	_ray_set_info_next(t_ray *ray, t_game *game)
{
	if (ray->_dir.x < 0)
	{
		ray->_step.x = -1;
		ray->_length1d.x = (game->pos.x - (ray->_mpos.x)) * ray->_ssize.x;
	}
	else
	{
		ray->_step.x = 1;
		ray->_length1d.x = ((ray->_mpos.x + 1.0) - game->pos.x) * ray->_ssize.x;
	}
	if (ray->_dir.y < 0)
	{
		ray->_step.y = -1;
		ray->_length1d.y = (game->pos.y - (ray->_mpos.y)) * ray->_ssize.y;
	}
	else
	{
		ray->_step.y = 1;
		ray->_length1d.y = ((ray->_mpos.y + 1.0) - game->pos.y) * ray->_ssize.y;
	}
}

void	_ray_set_info(t_f64 direction, t_ray *ray, t_game *game)
{
	mem_set_zero(ray, sizeof(*ray));
	while (direction > PI)
		direction -= 2 * PI;
	while (direction < -PI)
		direction += 2 * PI;
	ray->direction = direction;
	ray->_dir = vf2d(cos(direction), sin(direction));
	ray->_mpos = vi2d(game->pos.x, game->pos.y);
	ray->_ssize = vf2d(sqrt(1.0 + (ray->_dir.y / ray->_dir.x) * (ray->_dir.y
					/ ray->_dir.x)), sqrt(1.0 + (ray->_dir.x / ray->_dir.y)
				* (ray->_dir.x / ray->_dir.y)));
	_ray_set_info_next(ray, game);
}

void	_ray_perform_step(t_ray *ray, t_game *game)
{
	if (ray->_length1d.x < ray->_length1d.y)
	{
		ray->_mpos.x += ray->_step.x;
		ray->ray_len = ray->_length1d.x;
		ray->_length1d.x += ray->_ssize.x;
	}
	else
	{
		ray->_mpos.y += ray->_step.y;
		ray->ray_len = ray->_length1d.y;
		ray->_length1d.y += ray->_ssize.y;
	}
	ray->x = cos(ray->direction) * ray->ray_len + game->pos.x;
	ray->y = sin(ray->direction) * ray->ray_len + game->pos.y;
}

t_ray	my_ray(t_game *game, double direction, bool check_doors)
{
	t_ray	ray;

	_ray_set_info(direction, &ray, game);
	while (ray.ray_len < MAX_DIST)
	{
		_ray_perform_step(&ray, game);
		if (get_tile(&game->map, ray._mpos) & TILE_SOLID || \
			(check_doors && get_tile(&game->map, ray._mpos) & TILE_DOOR))
		{
			ray.hit_wall = true;
			ray.tile = get_tile(&game->map, ray._mpos);
			ray.tile_pos = ray._mpos;
			hit_x_y(&ray);
			break ;
		}
	}
	if (ray.ray_len >= MAX_DIST)
	{
		ray.ray_len = MAX_DIST;
		ray.hit_wall = true;
		ray.tile = TILE_EMPTY;
		ray.tex = TEX_NONE;
	}
	return (ray);
}
