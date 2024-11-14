/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:41:01 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/14 11:57:15 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/textures.h"
#include "app/tile.h"

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
