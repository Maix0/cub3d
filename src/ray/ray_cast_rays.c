/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_rays.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:45:45 by lgasqui           #+#    #+#             */
/*   Updated: 2024/11/25 14:25:36 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/textures.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/blx/colors.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/vec2/vec2.h"
#include <math.h>

t_ray	my_ray(t_game *game, double direction, bool check_door);

void	ray_tex_door(t_game *game, t_ray *ray)
{
	if (ray->tile & TILE_DOOR)
	{
		if (game->door_timer >= 0 && game->door_timer < 1.0)
			ray->tex = TEX_DOOR_T1;
		else if (game->door_timer >= 1.0 && game->door_timer < 2.0)
			ray->tex = TEX_DOOR_T2;
		else if (game->door_timer >= 2.0 && game->door_timer < 3.0)
			ray->tex = TEX_DOOR_T3;
		else if (game->door_timer >= 3.0 && game->door_timer < 4.0)
			ray->tex = TEX_DOOR_T4;
	}
}

void	draw_ray(t_blx *ctx, t_ray *ray, t_vi2d pos, t_f64 ceiling)
{
	t_color		col;
	t_sprite	*texture;
	t_vf2d		tex_pos;
	t_game		*game;

	game = ctx->app.data;
	if (pos.y <= ceiling)
		blx_draw(ctx, pos, game->map.info.ceiling_color);
	else if ((double)pos.y <= ctx->app.size_y - ceiling)
	{
		col = new_color(0, 0, 0);
		texture = hmap_texture_get(game->textures, &ray->tex);
		tex_pos = vf2d(ray->percent_wall, (pos.y - ((double)ceiling)) \
			/ (((double)ctx->app.size_y - ceiling) - ((double)ceiling)));
		sprite_get_pixel_normalized(texture, tex_pos, &col);
		blx_draw(ctx, pos, col);
	}
	else
		blx_draw(ctx, pos, game->map.info.floor_color);
}

void	cast_rays(t_blx *ctx, t_game *game)
{
	double	angle;
	int		x;
	int		y;
	t_ray	ray;

	x = 0;
	while (x < (int)ctx->app.size_x)
	{
		y = 0;
		angle = (game->angle - game->fov / 2.0) + \
		(x / (double)ctx->app.size_x) * game->fov;
		ray = my_ray(game, angle, false);
		ray.ray_len *= cos(angle - game->angle);
		ray_tex_door(game, &ray);
		while (y < (int)ctx->app.size_y)
			draw_ray(ctx, &ray, vi2d(x, y++), (((double)ctx->app.size_y / 2.0) \
				- ((double)ctx->app.size_y / ray.ray_len)));
		x++;
	}
}
