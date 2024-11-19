/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 18:20:39 by maiboyer         ###   ########.fr       */
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
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec/vec_str.h"
#include "me/vec/vec_tile.h"
#include "me/vec2/vec2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

bool hit_x_y(t_ray *ray)
{
	double int_y;
	double int_x;
	t_vf2d pos;

	ray->tex = TEX_NORTH;
	pos.x = modf(ray->x, &int_x);
	pos.y = modf(ray->y, &int_y);

	double distance_x = fmin(pos.x, 1 - pos.x);
	double distance_y = fmin(pos.y, 1 - pos.y);

	if (distance_x >= distance_y)
	{
		if (ray->direction >= 0)
		{
			ray->tex = TEX_SOUTH;
			ray->percent_wall = 1 - pos.x;
		}
		else
		{
			ray->tex = TEX_NORTH;
			ray->percent_wall = pos.x;
		}
	}
	else
	{
		if (ray->direction >= PI / 2 || ray->direction <= -PI / 2)
		{
			ray->tex = TEX_WEST;
			ray->percent_wall = 1 - pos.y;
		}
		else
		{
			ray->percent_wall = pos.y;
			ray->tex = TEX_EAST;
		}
	}
	return false;
}
#define MAX_DIST 250.0

t_ray my_ray(t_game *game, double direction, bool check_door)
{
	t_ray ray;
	ray.ray_len = 0;
	ray.x = 0; // cos
	ray.y = 0; // sin
	while (direction > PI)
		direction -= 2 * PI;
	while (direction < -PI)
		direction += 2 * PI;
	ray.direction = direction;
	t_vf2d ray_dir = vf2d(cos(direction), sin(direction));
	t_vf2d ray_step_size =
		vf2d(sqrt(1.0 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)),
			 sqrt(1.0 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y)));
	t_vf2d ray_length1d = vf2d(0.0, 0.0);
	t_vi2d step = vi2d(0, 0);
	t_vi2d map_check = vi2d(game->pos.x, game->pos.y);

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
		//ray.ray_len += 0.01;
		ray.x = cos(direction) * ray.ray_len + game->pos.x;
		ray.y = sin(direction) * ray.ray_len + game->pos.y;
		// quelle tile je touche, len du ray, et sud est ,
		if (get_tile(&game->map, map_check) & TILE_SOLID ||
			(check_door &&
			 get_tile(&game->map, map_check) & TILE_DOOR))
		{
			ray.hit_wall = true;
			ray.tile = get_tile(&game->map, map_check);
			hit_x_y(&ray);
			break;
		}
	}
	if (ray.ray_len >= MAX_DIST)
	{
		ray.ray_len = MAX_DIST;
		ray.hit_wall = false;
	}
	return (ray);
}

void cast_rays(t_blx *ctx, t_game *game)
{
	int i;
	int y;

	i = 0;
	while (i < (int)ctx->app.size_x)
	{
		double angle;
		t_ray ray;
		int ceiling;
		int floor;
		
		y = 0;
		angle = (game->angle - game->fov / 2.0) + (i / (double)ctx->app.size_x) * game->fov;
		ray = my_ray(game, angle, false);
		ray.ray_len *= cos(angle - game->angle);
		ceiling = (((double)ctx->app.size_y / 2.0) -
					   ((double)ctx->app.size_y / ray.ray_len) * ray.hit_wall);
		floor = (double)ctx->app.size_y - ceiling;
		if (ray.tile & TILE_DOOR)
			ray.tex = TEX_DOOR;
		while (y < (int)ctx->app.size_y)
		{
			if (y <= ceiling)
				blx_draw(ctx, vi2d(i, y), game->map.info.ceiling_color);
			else if (y <= floor)
			{
				t_vf2d	  tex_pos;
				t_sprite *texture;
				t_color col;

				texture = hmap_texture_get(game->textures, &ray.tex);
				tex_pos = vf2d(ray.percent_wall,
							   (y - ((double)ceiling)) /
								   (((double)floor) - ((double)ceiling)));
				col = new_color(0,0,0);
				sprite_get_pixel_normalized(texture, tex_pos, &col);
				blx_draw(ctx, vi2d(i, y), col);
			}
			else
				blx_draw(ctx, vi2d(i, y), game->map.info.floor_color);
			y++;
		}
		i++;
	}
}

void handle_door(t_blx *ctx, t_game *game, t_vi2d pos)
{
	t_tile door_tile;

	if (!BONUS)
		return;
	(void)(ctx);
	if (!is_key_released(ctx, KB_space))
		return;
	door_tile = get_tile(&game->map, pos);
	if (door_tile & TILE_SOLID)
		door_tile &= ~TILE_SOLID;
	else
		door_tile |= TILE_SOLID;
	set_tile(&game->map, pos, door_tile);
}

bool game_loop(t_blx *ctx)
{
	t_game	*game;
	t_ray ray;

	game = ctx->app.data;
	if (game->exit)
		return (true);
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	if (handle_input(ctx, game))
		return (true);
	cast_rays(ctx, game);
	{
		snprintf(game->str.buf, 1024, "FPS: %02.2f\nx: %.1f\ny: %.1f\n[a/d] angle: %.0f\n[i/o/p] FOV: %.0f",
				 1. / ctx->elapsed, game->pos.x, game->pos.y, game->angle/ (2.0 * PI) * 360, game->fov / (2.0 * PI) * 360);
		blx_fill_rect(ctx, vi2d(0, ctx->app.size_y - 8 * 5),
					  vi2d(8 * 16, ctx->app.size_y), new_color(0, 0, 0));
		blx_draw_string(ctx, vi2d(0, ctx->app.size_y - 8 * 5), game->str.buf,
						new_color(255, 255, 255));
		string_clear(&game->str);
	}
	ray = my_ray(game, game->angle, true);
	if (BONUS && (ray.tile & TILE_DOOR) && ray.ray_len < 1.0 &&
		ray.ray_len > INC)
		handle_door(ctx, game, vi2d(ray.x, ray.y));
	if (BONUS)
		draw_minimap(ctx, game);
	return (false);
}

void game_free(t_game *game)
{
	vec_tile_free(game->map.inner);
	hmap_texture_path_free(game->map.info.textures_path);
	hmap_texture_free(game->textures);
	string_free(game->str);
}

void game_free_blx(t_blx_app app)
{
	game_free(app.data);
}

int real_main(int argc, t_str argv[])
{
	t_game game;
	t_blx  blx;

	(void)(&argv[argc]);
	if (argc != 2)
		return (cube_error("Usage: %s <map>", argv[0]), 1);
	if (init_game(&game))
		return (cube_error("Failed allocation"), 1);
	mem_set_zero(&blx, sizeof(blx));
	if (parse_map(&game, argv[1]))
		return (game_free(&game), 1);
	if (blx_initialize(game_loop, game_free_blx,
					   (t_blx_app){
						   .size_x = 900,
						   .size_y = 700,
						   .pixel_size = 1,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		return (cube_error("Failed to init mlx"), game_free(&game), 1);
	if (fetch_textures(&blx))
		game.exit = true;
	blx_run(blx);
	return (0);
}

int main(int argc, char **argv)
{
	me_exit(real_main(argc, argv));
}
