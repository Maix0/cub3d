/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 14:05:53 by maiboyer         ###   ########.fr       */
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

#define ROTATE_SPEED (PI * 2)
#define SPEED 4
#define CELLSIZE 50
#define PLAYERSIZE 0.2

#define INC 0.2

bool is_wall_for_player(t_game *game, t_vf2d pos)
{
	bool is_wall;

	is_wall = false;
	// point itself
	is_wall |= get_tile(&game->map, vi2d(pos.x, pos.y)) & TILE_SOLID;

	// four neighbor points
	is_wall |= get_tile(&game->map, vi2d(pos.x + INC, pos.y)) & TILE_SOLID;
	is_wall |= get_tile(&game->map, vi2d(pos.x - INC, pos.y)) & TILE_SOLID;
	is_wall |= get_tile(&game->map, vi2d(pos.x, pos.y + INC)) & TILE_SOLID;
	is_wall |= get_tile(&game->map, vi2d(pos.x, pos.y - INC)) & TILE_SOLID;

	// four diagonally neighbor points
	is_wall |=
		get_tile(&game->map, vi2d(pos.x + INC, pos.y + INC)) & TILE_SOLID;
	is_wall |=
		get_tile(&game->map, vi2d(pos.x + INC, pos.y - INC)) & TILE_SOLID;
	is_wall |=
		get_tile(&game->map, vi2d(pos.x - INC, pos.y + INC)) & TILE_SOLID;
	is_wall |=
		get_tile(&game->map, vi2d(pos.x - INC, pos.y - INC)) & TILE_SOLID;

	// it basically makes a square of size 2*INC centered around `pos`
	// meaning that if any point is inside a wall, the point will be considered
	// inside the wall. this means that we can't get closer than INC to a wall
	return (is_wall);
}

void perform_collision(t_blx *ctx, t_game *game)
{
	(void)(ctx);
	if (!is_wall_for_player(game, vf2d(game->new_pos.x, game->pos.y)))
		game->pos.x = game->new_pos.x;
	if (!is_wall_for_player(game, vf2d(game->pos.x, game->new_pos.y)))
		game->pos.y = game->new_pos.y;
}

bool handle_input(t_blx *ctx, t_game *game)
{
	game->new_pos = game->pos;
	if (is_key_held(ctx, KB_Escape))
		return (true);
	if (is_key_held(ctx, KB_w) || is_key_held(ctx, KB_Up))
	{
		game->new_pos.x += cos(game->angle) * SPEED * ctx->elapsed;
		game->new_pos.y += sin(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_s) || is_key_held(ctx, KB_Down))
	{
		game->new_pos.x -= cos(game->angle) * SPEED * ctx->elapsed;
		game->new_pos.y -= sin(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_q))
	{
		game->new_pos.x += sin(game->angle) * SPEED * ctx->elapsed;
		game->new_pos.y -= cos(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_e))
	{
		game->new_pos.x -= sin(game->angle) * SPEED * ctx->elapsed;
		game->new_pos.y += cos(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_a) || is_key_held(ctx, KB_Left))
		game->angle -= ROTATE_SPEED * ctx->elapsed;
	if (is_key_held(ctx, KB_d) || is_key_held(ctx, KB_Right))
		game->angle += ROTATE_SPEED * ctx->elapsed;

	perform_collision(ctx, game);
	return (false);
}

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

void sanitize_input(t_blx *ctx, t_game *game)
{
	(void)(ctx);
	while (game->angle >= PI)
		game->angle -= 2.0 * PI;
	while (game->angle < -PI)
		game->angle += 2.0 * PI;
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

bool game_loop(t_blx *ctx)
{
	t_game	*game;
	t_string str;

	game = ctx->app.data;
	if (handle_input(ctx, game))
		return (true);
	sanitize_input(ctx, game);
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	draw_map(ctx, game);

	// TODO: remove this
	{
		str = string_new(1024);
		snprintf(str.buf, 1024, "FPS: %02.2f\nx: %f\ny: %f\nangle: %f",
				 1. / ctx->elapsed, game->pos.x, game->pos.y, game->angle);
		blx_draw_string(ctx, vi2d(0, 120), str.buf, new_color(255, 255, 255));
		string_free(str);
	}
	draw_player(ctx, game);
	return (false);
}

void game_free(t_blx_app app)
{
	t_game *game;
	game = app.data;

	vec_tile_free(game->map.inner);
	(void)(app);
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
		return (1);
	if (blx_initialize(game_loop, game_free,
					   (t_blx_app){
						   .size_x = game.map.size.x * CELLSIZE,
						   .size_y = game.map.size.y * CELLSIZE,
						   .pixel_size = 1,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		return (cube_error("Failed to init mlx"), 1);
	if (fetch_textures(&game))
		return (1);
	blx_run(blx);
	return (0);
}
