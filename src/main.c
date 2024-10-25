/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 15:11:21 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/string/string.h"
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
#define PLAYERSIZE 0.5

bool handle_input(t_blx *ctx, t_game *game)
{
	if (is_key_held(ctx, KB_Escape))
		return (true);
	if (is_key_held(ctx, KB_w) || is_key_held(ctx, KB_Up))
	{
		game->pos.x += cos(game->angle) * SPEED * ctx->elapsed;
		game->pos.y += sin(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_s) || is_key_held(ctx, KB_Down))
	{
		game->pos.x -= cos(game->angle) * SPEED * ctx->elapsed;
		game->pos.y -= sin(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_q))
	{
		game->pos.x += sin(game->angle) * SPEED * ctx->elapsed;
		game->pos.y -= cos(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_e))
	{
		game->pos.x -= sin(game->angle) * SPEED * ctx->elapsed;
		game->pos.y += cos(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_a) || is_key_held(ctx, KB_Left))
		game->angle -= ROTATE_SPEED * ctx->elapsed;
	if (is_key_held(ctx, KB_d) || is_key_held(ctx, KB_Right))
		game->angle += ROTATE_SPEED * ctx->elapsed;
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

bool game_loop(t_blx *ctx)
{
	t_game	*game;
	t_string str;

	game = ctx->app.data;
	if (handle_input(ctx, game))
		return (true);
	sanitize_input(ctx, game);
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	// TODO: remove this
	{
		str = string_new(1024);
		snprintf(str.buf, 1024, "x: %f\ny: %f\nangle: %f", game->pos.x, game->pos.y, game->angle);
		blx_draw_string(ctx, vi2d(0, 0), str.buf, new_color(255, 255, 255));
		string_free(str);
	}

	blx_fill_rect(ctx, vi2d(100, 50), vi2d(150, 250), new_color(255, 255, 255));
	draw_player(ctx, game);
	return (false);
}
void game_free(t_blx_app app)
{
	(void)(app);
}

int main(int argc, char **argv)
{
	t_game game;
	t_blx  blx;
	t_data data;
	
	if (argc != 2)
		return (printf("Error.\nInvalid argument.\n"), 1);
	if (!map_format(argv[1]))
		exit(1);
	mem_set_zero(&game, sizeof(game));
	mem_set_zero(&blx, sizeof(blx));
	game.map.size = vi2d(30, 20);
	game.pos.x = 5;
	game.pos.y = 5;
	if (blx_initialize(game_loop, game_free,
					   (t_blx_app){
						   .size_x = game.map.size.x * CELLSIZE,
						   .size_y = game.map.size.y * CELLSIZE,
						   .pixel_size = 1,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		exit(1);
	data.map = read_map(&data, argv[1], &blx);
	blx_run(blx);
	return (0);
}
