/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/14 15:09:32 by lgasqui          ###   ########.fr       */
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

	ray->tex = TEX_NORTH;
	x = modf(ray->x, &int_x);
	y = modf(ray->y, &int_y);

	double distance_x = fmin(x, 1 - x);
	double distance_y = fmin(y, 1 - y);

	if (distance_x > distance_y)
	{
		if (ray->direction > 0)	
		{
			ray->tex = TEX_SOUTH;
			ray->percent_wall = 1 - x;
		}
		else
		{
			ray->tex = TEX_NORTH;
			ray->percent_wall = x;
		}	
	}
	else
	{
		if (x >= 0 && x <= 0.5)
		{
			ray->tex = TEX_EAST;
			ray->percent_wall = y;
		}
		else
		{
			ray->percent_wall = 1 - y;
			ray->tex = TEX_WEST;
		}
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
		ray.ray_len += 0.005;
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
#define MAX_DIST 15.0

void cast_rays(t_blx *ctx, t_game *game)
{
	int i;
	int y;

	i = 0;

	while (i < (int)NUM_RAYS)
	{
		y = 0;
		//double x = (i / ctx->app.size_x - 0.5) ;
		//double angle = atan2(x * FOV, 0.8);
		double angle = (game->angle - FOV / 2.0) + (i / (double)ctx->app.size_x) * FOV; 
		t_ray  ray = my_ray(game, angle);

		//ray.ray_len *= cos(angle);
		
		int ceiling = (((double)ctx->app.size_y / 2.0) - (double)ctx->app.size_y / ray.ray_len);
		int floor = (double)ctx->app.size_y - ceiling;

		while(y < (int)ctx->app.size_y)
		{
			if(y <= ceiling)
			{
				blx_draw(ctx, vi2d(i, y), game->map.info.ceiling_color);
			}
			else if (y > ceiling && y <= floor)
			{
				t_vf2d tex_pos;
				t_sprite *texture;

				texture = hmap_texture_get(game->textures, &ray.tex);
				if (texture == NULL)
					me_abort("halp");
				tex_pos = vf2d(ray.percent_wall, (y - ((double)ceiling)) / (((double)floor) - ((double)ceiling)));
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

		// t_vi2d ray_end = vi2d(
		// 	cos(ray_angle) * ray.ray_len * CELLSIZE + game->pos.x * CELLSIZE,
		// 	sin(ray_angle) * ray.ray_len * CELLSIZE + game->pos.y * CELLSIZE);

		// blx_draw_line(ctx, vi2d(game->pos.x * CELLSIZE, game->pos.y * CELLSIZE),
		// 	ray_end, new_color(255, 0, 0));
		i++;
	}
}

void handle_door(t_blx *ctx, t_game *game, t_vi2d pos)
{
	t_tile door_tile;

	ctx->elapsed;
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
	//draw_map(ctx, game);

	if (handle_input(ctx, game))
		return (true);
	cast_rays(ctx, game);
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
	//draw_player(ctx, game);
	//t_vi2d endline = vi2d(
	//	cos(game->angle) * CELLSIZE * ray.ray_len + game->pos.x * CELLSIZE,
	//	sin(game->angle) * ray.ray_len * CELLSIZE + game->pos.y * CELLSIZE);
	//blx_draw_line(ctx, vi2d(game->pos.x * CELLSIZE, game->pos.y * CELLSIZE),
				  //endline, new_color(120, 255, 0));
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
						   .size_x = 900,
						   .size_y = 500,
						   .pixel_size = 1,
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
