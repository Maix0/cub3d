/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/18 22:10:53 by maiboyer         ###   ########.fr       */
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
#include "me/mem/_allocator.h"
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
			ray->tex = TEX_EAST;
			ray->percent_wall = 1 - pos.y;
		}
		else
		{
			ray->percent_wall = pos.y;
			ray->tex = TEX_WEST;
		}
	}
	return false;
}
#define MAX_DIST 500.0

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

	while (ray.ray_len < MAX_DIST)
	{
		ray.ray_len += 0.01;
		ray.x = cos(direction) * ray.ray_len + game->pos.x;
		ray.y = sin(direction) * ray.ray_len + game->pos.y;
		// quelle tile je touche, len du ray, et sud est ,
		if (get_tile(&game->map, vi2d(ray.x, ray.y)) & TILE_SOLID ||
			(check_door &&
			 get_tile(&game->map, vi2d(ray.x, ray.y)) & TILE_DOOR))
		{
			ray.hit_wall = true;
			ray.tile = get_tile(&game->map, vi2d(ray.x, ray.y));
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

	while (i < (int)NUM_RAYS)
	{
		y = 0;
		// double x = (i / ctx->app.size_x - 0.5) ;
		// double angle = atan2(x * FOV, 0.8);
		double angle =
			(game->angle - FOV / 2.0) + (i / (double)ctx->app.size_x) * FOV;
		t_ray ray = my_ray(game, angle, false);

		ray.ray_len *= cos(angle - game->angle);

		int ceiling = (((double)ctx->app.size_y / 2.0) -
					   ((double)ctx->app.size_y / ray.ray_len) * ray.hit_wall);
		int floor = (double)ctx->app.size_y - ceiling;
		if (ray.tile & TILE_DOOR)
			ray.tex = TEX_DOOR;
		while (y < (int)ctx->app.size_y)
		{
			if (y <= ceiling)
			{
				blx_draw(ctx, vi2d(i, y), game->map.info.ceiling_color);
			}
			else if (y > ceiling && y <= floor)
			{
				t_vf2d	  tex_pos;
				t_sprite *texture;

				texture = hmap_texture_get(game->textures, &ray.tex);
				if (texture == NULL)
					me_abort("halp");
				tex_pos = vf2d(ray.percent_wall,
							   (y - ((double)ceiling)) /
								   (((double)floor) - ((double)ceiling)));
				t_color col;

				if (sprite_get_pixel_normalized(texture, tex_pos, &col))
					me_abort("halp2");
				blx_draw(ctx, vi2d(i, y), col);
			}
			else
			{
				blx_draw(ctx, vi2d(i, y), game->map.info.floor_color);
			}
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
	t_string str;

	game = ctx->app.data;
	if (game->exit)
		return (true);
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	// draw_map(ctx, game);

	if (handle_input(ctx, game))
		return (true);
	cast_rays(ctx, game);
	t_ray ray = my_ray(game, game->angle, true);
	// TODO: remove this
	{
		str = string_new(1024);
		snprintf(str.buf, 1024, "FPS: %02.2f\nx: %f\ny: %f\nangle: %f",
				 1. / ctx->elapsed, game->pos.x, game->pos.y, game->angle);
		blx_fill_rect(ctx, vi2d(0, ctx->app.size_y - 8 * 4),
					  vi2d(8 * 16, ctx->app.size_y), new_color(0, 0, 0));
		blx_draw_string(ctx, vi2d(0, ctx->app.size_y - 8 * 4), str.buf,
						new_color(255, 255, 255));
		string_free(str);
	}
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
}

void game_free_blx(t_blx_app app)
{
	game_free(app.data);
}

int main(int argc, char **argv)
{
	t_game game;
	t_blx  blx;

	(void)(&argv[argc]);
	if (argc != 2)
		return (cube_error("Usage: %s <map>", argv[0]), 1);
	init_game(&game);
	mem_set_zero(&blx, sizeof(blx));
	if (parse_map(&game, argv[1]))
		return (game_free(&game), uninit_global_allocator(), 1);
	if (blx_initialize(game_loop, game_free_blx,
					   (t_blx_app){
						   .size_x = 900,
						   .size_y = 700,
						   .pixel_size = 1,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		return (cube_error("Failed to init mlx"), game_free(&game),
				uninit_global_allocator(), 1);
	if (fetch_textures(&blx))
		game.exit = true;
	blx_run(blx);
	return (0);
}
