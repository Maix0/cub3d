/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/03 20:14:36 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
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

t_f64 f64_clamp(t_f64 this, t_f64 min, t_f64 max)
{
	if (this != this)
		return (this);
	if (this > max)
		return (max);
	if (this < min)
		return (min);
	return (this);
}

#define PI 3.14159265358979323846264338327950288
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
	is_wall |= get_tile(&game->map, vi2d(pos.x + INC, pos.y + INC)) & TILE_SOLID;
	is_wall |= get_tile(&game->map, vi2d(pos.x + INC, pos.y - INC)) & TILE_SOLID;
	is_wall |= get_tile(&game->map, vi2d(pos.x - INC, pos.y + INC)) & TILE_SOLID;
	is_wall |= get_tile(&game->map, vi2d(pos.x - INC, pos.y - INC)) & TILE_SOLID;

	// it basically makes a square of size 2*INC centered around `pos`
	// meaning that if any point is inside a wall, the point will be considered inside the wall.
	// this means that we can't get closer than INC to a wall
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
		vi2d(((game->pos.x + (cos(game->angle)) * (t_f64)PLAYERSIZE) * (t_f64)CELLSIZE),
			 ((game->pos.y + (sin(game->angle)) * (t_f64)PLAYERSIZE) * (t_f64)CELLSIZE));
	blx_draw_line(ctx, vi2d((game->pos.x) * (t_f64)CELLSIZE, (game->pos.y) * (t_f64)CELLSIZE),
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
	game->pos.x = f64_clamp(game->pos.x, PLAYERSIZE, (t_f64)game->map.size.x - PLAYERSIZE);
	game->pos.y = f64_clamp(game->pos.y, PLAYERSIZE, (t_f64)game->map.size.y - PLAYERSIZE);
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
				fill = new_color(0x1E, 0x1E, 0x1E);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_WALL | TILE_SOLID))
			{
				fill = new_color(127, 127, 127);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_WALL))
			{
				fill = new_color(0x1E, 0x1E, 0x1E);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else
			{
				fill = new_color(0, 0, 255);
				border = new_color(0, 0, 255);
			}

			blx_fill_rect(ctx, vi2d(tilepos.x * CELLSIZE, tilepos.y * CELLSIZE),
						  vi2d((tilepos.x + 1) * CELLSIZE - 1, (tilepos.y + 1) * CELLSIZE - 1),
						  fill);
			blx_draw_rect(ctx, vi2d(tilepos.x * CELLSIZE, tilepos.y * CELLSIZE),
						  vi2d((tilepos.x + 1) * CELLSIZE - 1, (tilepos.y + 1) * CELLSIZE - 1),
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
		snprintf(str.buf, 1024, "x: %f\ny: %f\nangle: %f", game->pos.x, game->pos.y, game->angle);
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

	vec_tile_free(game->map.map);
	(void)(app);
}

void create_test_map(t_game *game)
{
	t_vec_str	lines;
	t_usize		y;
	t_usize		x;
	t_const_str map;
	t_usize		max_width;

	map = "   11111   \n"
		  "   10S01   \n"
		  "  1100011  \n"
		  "11100000111\n"
		  "10001010001\n"
		  "10000100001\n"
		  "10001010001\n"
		  "11100000111\n"
		  "  1100011  \n"
		  "   10001   \n"
		  "   11111   \n";
	if (str_split(map, "\n", &lines))
		me_abort("Unable to split temp map");
	y = 0;
	max_width = 0;
	while (y < lines.len)
	{
		if (str_len(lines.buffer[y]) > max_width)
			max_width = str_len(lines.buffer[y]);
		y++;
	}
	printf("map.len = %zu\n", lines.len);
	game->map.size.x = max_width;
	game->map.size.y = lines.len;
	game->map.map = vec_tile_new(max_width * lines.len, NULL);
	y = 0;
	t_usize player_spawn_count;
	player_spawn_count = 0;
	while (y < lines.len)
	{
		x = 0;
		while (lines.buffer[y][x] != 0)
		{
			char tile = lines.buffer[y][x];
			if (tile == '0')
				vec_tile_push(&game->map.map, TILE_FLOOR);
			else if (tile == '1')
				vec_tile_push(&game->map.map, TILE_WALL | TILE_SOLID);
			else if (tile == ' ')
				vec_tile_push(&game->map.map, TILE_WALL);
			else if (tile == 'N')
			{
				player_spawn_count++;
				game->pos.x = x + 0.5;
				game->pos.y = y + 0.5;
				game->angle = -PI / 2;
				vec_tile_push(&game->map.map, TILE_FLOOR);
			}
			else if (tile == 'S')
			{
				player_spawn_count++;
				game->pos.x = x + 0.5;
				game->pos.y = y + 0.5;
				game->angle = PI / 2;
				vec_tile_push(&game->map.map, TILE_FLOOR);
			}
			else if (tile == 'W')
			{
				player_spawn_count++;
				game->pos.x = x + 0.5;
				game->pos.y = y + 0.5;
				game->angle = PI;
				vec_tile_push(&game->map.map, TILE_FLOOR);
			}
			else if (tile == 'E')
			{
				player_spawn_count++;
				game->pos.x = x + 0.5;
				game->pos.y = y + 0.5;
				game->angle = 0;
				vec_tile_push(&game->map.map, TILE_FLOOR);
			}
			else
				me_abort("invalid map character");
			x++;
		}
		while (x++ < max_width)
			vec_tile_push(&game->map.map, TILE_WALL);
		y++;
	}
	vec_str_free(lines);
	if (player_spawn_count != 1)
		me_abort("too may spawns");
}

int main(int argc, char **argv)
{
	t_game game;
	t_blx  blx;

	(void)(&argv[argc]);
	// if (argc != 2)
	//	return (cube_error("Usage: %s <map>", argv[0]), 1);
	// if (map_format(argv[1]))
	//	return (1);
	mem_set_zero(&game, sizeof(game));
	mem_set_zero(&blx, sizeof(blx));
	create_test_map(&game);
	printf("game->map.size = (%i, %i);\n", game.map.size.x, game.map.size.y);
	if (blx_initialize(game_loop, game_free,
					   (t_blx_app){
						   .size_x = game.map.size.x * CELLSIZE,
						   .size_y = game.map.size.y * CELLSIZE,
						   .pixel_size = 2,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		exit(1);
	// if (read_map(&game.map, argv[1], &blx))
	//	return (blx_free(blx), 1);
	blx_run(blx);
	return (0);
}
