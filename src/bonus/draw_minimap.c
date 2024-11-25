/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 21:18:51 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/25 14:25:23 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/colors.h"
#include "me/types.h"
#include <math.h>

#define C_SIZE 20
#define P_SIZE 0.48
#define M_SIZE 10

// Dans la minimap set up le cercle du player et la direction de son rayon
static inline void	draw_player(t_blx *ctx, t_game *game)
{
	t_vi2d	endline;

	endline = \
	vi2d(((M_SIZE + (cos(game->angle)) * (t_f64)P_SIZE) * \
	(t_f64)C_SIZE), ((M_SIZE + (sin(game->angle)) * (t_f64)P_SIZE) * \
	(t_f64)C_SIZE));
	blx_draw_line(\
		ctx, vi2d(M_SIZE * (t_f64)C_SIZE, M_SIZE * (t_f64)C_SIZE), \
		endline, new_color(0, 255, 0));
	blx_draw_circle(ctx, vi2d(M_SIZE * C_SIZE, M_SIZE * C_SIZE), \
					(P_SIZE * C_SIZE) - 1, new_color(255, 0, 0));
	blx_draw(ctx, endline, new_color(0, 0, 255));
}

//set up des couleurs de la grid de la minimap falses = ne dessine pas
static inline bool	_minimap_get_col(\
				t_tile tile, t_color *fill, t_color *border)
{
	if (tile == (TILE_FLOOR))
	{
		*fill = new_color(0x1E, 0x1E, 0x1E);
		*border = new_color(0xE1, 0xE1, 0xE1);
	}
	else if (tile == (TILE_WALL))
	{
		*fill = new_color(127, 127, 127);
		*border = new_color(0xE1, 0xE1, 0xE1);
	}
	else if (tile == (TILE_DOOR | TILE_SOLID))
	{
		*fill = new_color(0xff, 0x00, 0x00);
		*border = new_color(0xE1, 0xE1, 0xE1);
	}
	else if (tile == (TILE_DOOR))
	{
		*fill = new_color(0x00, 0xff, 0x00);
		*border = new_color(0xE1, 0xE1, 0xE1);
	}
	else
		return (false);
	return (true);
}
// globalement dessine un carre de 10x10 autour du joueur

void	draw_minimap(t_blx *ctx, t_game *game)
{
	t_vi2d	pos;
	t_color	fill;
	t_color	border;

	pos = vi2d(game->pos.x - M_SIZE, game->pos.y - M_SIZE);
	while (pos.y < game->pos.y + M_SIZE)
	{
		pos.x = game->pos.x - M_SIZE;
		while (pos.x < game->pos.x + M_SIZE)
		{
			if (_minimap_get_col(get_tile(&game->map, pos), &fill, &border))
				(blx_fill_rect(ctx, vi2d((pos.x - game->pos.x + M_SIZE) * \
	C_SIZE, (pos.y - game->pos.y + M_SIZE) * C_SIZE), vi2d(((pos.x - \
	game->pos.x + M_SIZE) + 1) * C_SIZE - 1, ((pos.y - game->pos.y + M_SIZE) + \
	1) * C_SIZE - 1), fill), blx_draw_rect(ctx, vi2d((pos.x - game->pos.x + \
	M_SIZE) * C_SIZE, (pos.y - game->pos.y + M_SIZE) * C_SIZE), vi2d((pos.x - \
	game->pos.x + M_SIZE + 1) * C_SIZE - 1, (pos.y - game->pos.y + M_SIZE + 1) \
	* C_SIZE - 1), border));
			pos.x++;
		}
		pos.y++;
	}
	draw_player(ctx, game);
}
