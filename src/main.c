/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/14 14:03:38 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
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

#define CELLSIZE 50
#define PLAYERSIZE 0.2

void draw_player(t_blx *ctx, t_game *game)
{
	t_vi2d endline =
		vi2d(((game->pos.x + (cos(game->angle)) * (t_f64)PLAYERSIZE) *
			  (t_f64)CELLSIZE),
			 ((game->pos.y + (sin(game->angle)) * (t_f64)PLAYERSIZE) *
			  (t_f64)CELLSIZE));
	blx_draw_line(
		ctx,
		vi2d((game->pos.x) * (t_f64)CELLSIZE, (game->pos.y) * (t_f64)CELLSIZE),
		endline, new_color(0, 255, 0));
	blx_draw_circle(ctx, vi2d(game->pos.x * CELLSIZE, game->pos.y * CELLSIZE),
					(PLAYERSIZE * CELLSIZE) - 1, new_color(255, 0, 0));
	blx_draw(ctx, endline, new_color(0, 0, 255));
}

void draw_map(t_blx *ctx, t_game *game)
{
	t_vi2d	tilepos;
	t_tile	tile;
	t_color fill;
	t_color border;

	tilepos = vi2d(0, 0);
	while (tilepos.y < game->map.size.y)
	{
		tilepos.x = 0;
		while (tilepos.x < game->map.size.x)
		{
			tile = get_tile(&game->map, tilepos);
			if (tile == TILE_EMPTY)
			{
				fill = new_color(255, 0, 0);
				border = new_color(0, 0, 0);
			}
			else if (tile == (TILE_FLOOR))
			{
				// fill = new_color(0x1E, 0x1E, 0x1E);
				fill = game->map.info.floor_color;
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_WALL))
			{
				// fill = new_color(127, 127, 127);
				fill = game->map.info.ceiling_color;
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_SOLID))
			{
				fill = new_color(0x00, 0x00, 0x00);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_DOOR | TILE_SOLID))
			{
				fill = new_color(0xff, 0x00, 0x00);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_DOOR))
			{
				fill = new_color(0x00, 0xff, 0x00);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else
			{
				fill = new_color(0, 0, 255);
				border = new_color(0, 0, 255);
			}

			blx_fill_rect(ctx, vi2d(tilepos.x * CELLSIZE, tilepos.y * CELLSIZE),
						  vi2d((tilepos.x + 1) * CELLSIZE - 1,
							   (tilepos.y + 1) * CELLSIZE - 1),
						  fill);
			blx_draw_rect(ctx, vi2d(tilepos.x * CELLSIZE, tilepos.y * CELLSIZE),
						  vi2d((tilepos.x + 1) * CELLSIZE - 1,
							   (tilepos.y + 1) * CELLSIZE - 1),
						  border);
			tilepos.x++;
		}
		tilepos.y++;
	}
}

bool hit_x_y(t_ray *ray)
{
	double int_y;
	double int_x;
	double x;
	double y;

	ray->nord = 0;
	ray->sud = 0;
	ray->est = 0;
	ray->west = 0;
	x = modf(ray->x, &int_x);
	y = modf(ray->y, &int_y);

	double distance_x = fmin(x, 1 - x);
	double distance_y = fmin(y, 1 - y);

	if (distance_x > distance_y)
	{
		if (ray->direction > 0)
			ray->sud = 1;
		else
			ray->nord = 1;
	}
	else
	{
		if (x >= 0 && x <= 0.5)
			ray->est = 1;

		else
			ray->west = 1;
	}
	return false;
}

t_ray my_ray(t_game *game, double direction)
{
	t_ray ray;
	ray.ray_len = 0;
	ray.x = 0; // cos
	ray.y = 0; // sin
	ray.direction = direction;

	while (ray.ray_len < 50)
	{
		ray.ray_len += 0.01;
		ray.x = cos(direction) * ray.ray_len + game->pos.x;
		ray.y = sin(direction) * ray.ray_len + game->pos.y;
		// quelle tile je touche, len du ray, et sud est ,
		if (get_tile(&game->map, vi2d(ray.x, ray.y)) & TILE_SOLID ||
			get_tile(&game->map, vi2d(ray.x, ray.y)) & TILE_DOOR)
		{
			ray.tile = get_tile(&game->map, vi2d(ray.x, ray.y));
			hit_x_y(&ray);
			// printf("hit wall at [ray = %f] %f %f\n",ray,  mx, my);
			break;
		}
	}
	return (ray);
}

void cast_rays(t_blx *ctx, t_game *game)
{
	double start_angle = game->angle - (FOV * PI / 180.0) / 2;
	double angle_step = (FOV * PI / 180.0) / NUM_RAYS;

	for (t_u32 i = 0; i < NUM_RAYS; i++)
	{
		double ray_angle = start_angle + i * angle_step;
		t_ray  ray = my_ray(game, ray_angle);

		t_vi2d ray_end = vi2d(
			cos(ray_angle) * ray.ray_len * CELLSIZE + game->pos.x * CELLSIZE,
			sin(ray_angle) * ray.ray_len * CELLSIZE + game->pos.y * CELLSIZE);

		blx_draw_line(ctx, vi2d(game->pos.x * CELLSIZE, game->pos.y * CELLSIZE),
					  ray_end, new_color(255, 0, 0));
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
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	draw_map(ctx, game);

	if (handle_input(ctx, game))
		return (true);
	// cast_rays(ctx, game);
	t_ray ray = my_ray(game, game->angle);
	// TODO: remove this
	{
		str = string_new(1024);
		snprintf(str.buf, 1024, "FPS: %02.2f\nx: %f\ny: %f\nangle: %f",
				 1. / ctx->elapsed, game->pos.x, game->pos.y, game->angle);
		blx_draw_string(ctx, vi2d(0, 120), str.buf, new_color(255, 255, 255));
		string_free(str);
	}
	if (BONUS && (ray.tile & TILE_DOOR) && ray.ray_len < 1.0 &&
		ray.ray_len > INC)
		handle_door(ctx, game, vi2d(ray.x, ray.y));
	draw_player(ctx, game);
	t_vi2d endline = vi2d(
		cos(game->angle) * CELLSIZE * ray.ray_len + game->pos.x * CELLSIZE,
		sin(game->angle) * ray.ray_len * CELLSIZE + game->pos.y * CELLSIZE);
	blx_draw_line(ctx, vi2d(game->pos.x * CELLSIZE, game->pos.y * CELLSIZE),
				  endline, new_color(120, 255, 0));
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

void printdata(t_game *game)
{
	printf("PLAYER X = [%f]\n", game->pos.x);
	printf("PLAYER Y = [%f]\n", game->pos.y);
	printf("DIRECTION = [%f]\n", game->angle * 180 / PI);
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
		return (game_free(&game), 1);
	if (blx_initialize(game_loop, game_free_blx,
					   (t_blx_app){
						   .size_x = game.map.size.x * CELLSIZE,
						   .size_y = game.map.size.y * CELLSIZE,
						   .pixel_size = 2,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		return (cube_error("Failed to init mlx"), 1);
	if (fetch_textures(&blx))
		return (game_free(&game), 1);
	blx_run(blx);
	return (0);
}
