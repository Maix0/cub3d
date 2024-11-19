/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:18:51 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/18 21:20:35 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/colors.h"
#include "me/types.h"
#include <math.h>

#define CELLSIZE 20
#define PLAYERSIZE 0.48
#define MINIMAPSIZE 5

void draw_player(t_blx *ctx, t_game *game)
{
	t_vi2d endline =
		vi2d(((MINIMAPSIZE + (cos(game->angle)) * (t_f64)PLAYERSIZE) *
			  (t_f64)CELLSIZE),
			 ((MINIMAPSIZE + (sin(game->angle)) * (t_f64)PLAYERSIZE) *
			  (t_f64)CELLSIZE));
	blx_draw_line(
		ctx, vi2d(MINIMAPSIZE * (t_f64)CELLSIZE, MINIMAPSIZE * (t_f64)CELLSIZE),
		endline, new_color(0, 255, 0));
	blx_draw_circle(ctx, vi2d(MINIMAPSIZE * CELLSIZE, MINIMAPSIZE * CELLSIZE),
					(PLAYERSIZE * CELLSIZE) - 1, new_color(255, 0, 0));
	blx_draw(ctx, endline, new_color(0, 0, 255));
}

void draw_minimap(t_blx *ctx, t_game *game)
{
	t_vi2d	pos;
	t_tile	tile;
	t_color fill;
	t_color border;

	pos = vi2d(game->pos.x - MINIMAPSIZE, game->pos.y - MINIMAPSIZE);
	while (pos.y < game->pos.y + MINIMAPSIZE)
	{
		pos.x = game->pos.x - MINIMAPSIZE;
		while (pos.x < game->pos.x + MINIMAPSIZE)
		{
			tile = get_tile(&game->map, pos);
			fill = new_color_with_alpha(0, 0, 0, 255);
			if (tile == (TILE_FLOOR))
			{
				fill = new_color(0x1E, 0x1E, 0x1E);
				border = new_color(0xE1, 0xE1, 0xE1);
			}
			else if (tile == (TILE_WALL))
			{
				fill = new_color(127, 127, 127);
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

			if (fill.a == 0)
			{
				blx_fill_rect(
					ctx,
					vi2d((pos.x - game->pos.x + MINIMAPSIZE) * CELLSIZE,
						 (pos.y - game->pos.y + MINIMAPSIZE) * CELLSIZE),
					vi2d(((pos.x - game->pos.x + MINIMAPSIZE) + 1) * CELLSIZE -
							 1,
						 ((pos.y - game->pos.y + MINIMAPSIZE) + 1) * CELLSIZE -
							 1),
					fill);
				blx_draw_rect(
					ctx,
					vi2d((pos.x - game->pos.x + MINIMAPSIZE) * CELLSIZE,
						 (pos.y - game->pos.y + MINIMAPSIZE) * CELLSIZE),
					vi2d((pos.x - game->pos.x + MINIMAPSIZE + 1) * CELLSIZE - 1,
						 (pos.y - game->pos.y + MINIMAPSIZE + 1) * CELLSIZE -
							 1),
					border);
			}
			pos.x++;
		}
		pos.y++;
	}
	draw_player(ctx, game);
}
