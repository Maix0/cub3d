/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:52:59 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/22 14:51:17 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/string/string.h"
#include "me/vec2/vec2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_game t_game;
struct s_game
{
	t_vf2d pos;
	t_f64  angle;
};

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
#define ROTATE_SPEED (PI)
#define SPEED 100

bool game_loop(t_blx *ctx)
{
	t_game	*game;
	t_string str;

	game = ctx->app.data;
	if (is_key_pressed(ctx, KB_Escape))
		return (true);
	str = string_new(1024);
	if (is_key_pressed(ctx, KB_w))
	{
		game->pos.x += sin(game->angle) * SPEED * ctx->elapsed;
		game->pos.y += cos(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_pressed(ctx, KB_s))
	{
		game->pos.x -= sin(game->angle) * SPEED * ctx->elapsed;
		game->pos.y -= cos(game->angle) * SPEED * ctx->elapsed;
	}
	if (is_key_pressed(ctx, KB_a))
		game->angle -= ROTATE_SPEED * ctx->elapsed;
	if (is_key_pressed(ctx, KB_d))
		game->angle += ROTATE_SPEED * ctx->elapsed;
	while (game->angle >= PI)
		game->angle -= 2.0 * PI;
	while (game->angle < -PI)
		game->angle += 2.0 * PI;
	game->pos.x = f64_clamp(game->pos.x, 0, ctx->app.size_x - 1);
	game->pos.y = f64_clamp(game->pos.y, 0, ctx->app.size_y - 1);
	snprintf(str.buf, 1024, "x: %f\ny: %f\nangle: %f", game->pos.x, game->pos.y, game->angle);
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	blx_draw_string(ctx, vi2d(0, 0), str.buf, new_color(255, 255, 255));
	blx_draw(ctx, vi2d(game->pos.x, game->pos.y), new_color(255, 0, 0));
	return (false);
}
void game_free(t_blx_app app)
{
	(void)(app);
}

int main(void)
{
	t_game game;
	t_blx  blx;

	mem_set_zero(&game, sizeof(game));
	mem_set_zero(&blx, sizeof(blx));
	if (blx_initialize(game_loop, game_free,
					   (t_blx_app){
						   .size_x = 200,
						   .size_y = 100,
						   .pixel_size = 5,
						   .title = "Cub3d - Yes",
						   .data = &game,
					   },
					   &blx))
		exit(1);
	blx_run(blx);
	return (0);
}
