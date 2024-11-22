/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 19:25:49 by maiboyer         ###   ########.fr       */
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

t_ray	my_ray(t_game *game, double direction, bool check_door)
{
	t_ray	ray;
	t_vf2d	ray_dir;
	t_vf2d	ray_step_size;
	t_vf2d	ray_length1d;
	t_vi2d	step;
	t_vi2d	map_check;

	ray.ray_len = 0;
	ray.x = 0;
	ray.y = 0;
	while (direction > PI)
		direction -= 2 * PI;
	while (direction < -PI)
		direction += 2 * PI;
	ray.direction = direction;
	ray_dir = vf2d(cos(direction), sin(direction));
	ray_step_size = vf2d(sqrt(1.0 + (ray_dir.y / ray_dir.x) * (ray_dir.y
					/ ray_dir.x)), sqrt(1.0 + (ray_dir.x / ray_dir.y)
				* (ray_dir.x / ray_dir.y)));
	ray_length1d = vf2d(0.0, 0.0);
	step = vi2d(0, 0);
	map_check = vi2d(game->pos.x, game->pos.y);
	if (ray_dir.x < 0)
	{
		step.x = -1;
		ray_length1d.x = (game->pos.x - (map_check.x)) * ray_step_size.x;
	}
	else
	{
		step.x = 1;
		ray_length1d.x = ((map_check.x + 1.0) - game->pos.x) * ray_step_size.x;
	}
	if (ray_dir.y < 0)
	{
		step.y = -1;
		ray_length1d.y = (game->pos.y - (map_check.y)) * ray_step_size.y;
	}
	else
	{
		step.y = 1;
		ray_length1d.y = ((map_check.y + 1.0) - game->pos.y) * ray_step_size.y;
	}
	while (ray.ray_len < MAX_DIST)
	{
		if (ray_length1d.x < ray_length1d.y)
		{
			map_check.x += step.x;
			ray.ray_len = ray_length1d.x;
			ray_length1d.x += ray_step_size.x;
		}
		else
		{
			map_check.y += step.y;
			ray.ray_len = ray_length1d.y;
			ray_length1d.y += ray_step_size.y;
		}
		ray.x = cos(direction) * ray.ray_len + game->pos.x;
		ray.y = sin(direction) * ray.ray_len + game->pos.y;
		if (get_tile(&game->map, map_check) & TILE_SOLID || (check_door
				&& get_tile(&game->map, map_check) & TILE_DOOR))
		{
			ray.hit_wall = true;
			ray.tile = get_tile(&game->map, map_check);
			ray.tile_pos = map_check;
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

int	real_main(int argc, t_str argv[])
{
	t_game		game;
	t_blx		blx;
	t_blx_app	app_data;

	(void)(&argv[argc]);
	if (argc != 2)
		return (cube_error("Usage: %s <map>", argv[0]), 1);
	if (init_game(&game))
		return (cube_error("Failed allocation"), 1);
	mem_set_zero(&blx, sizeof(blx));
	if (parse_map(&game, argv[1]))
		return (game_free(&game), 1);
	app_data = (t_blx_app){.size_x = 900, .size_y = 700, .pixel_size = 1, \
		.title = "Cub3d - Yes", .data = &game};
	if (blx_initialize(game_loop, game_free_blx, app_data, &blx))
		return (cube_error("Failed to init mlx"), game_free(&game), 1);
	if (fetch_textures(&blx))
		game.exit = true;
	return (blx_run(blx), 0);
}

int	main(int argc, char **argv)
{
	me_exit(real_main(argc, argv));
}
