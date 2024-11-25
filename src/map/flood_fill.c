/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 20:58:12 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/25 13:12:14 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/tile.h"
#include "me/types.h"

void	tile_remove_floodfill_bit(t_usize index, t_tile *tile, void *ctx)
{
	(void)(index);
	(void)(ctx);
	*tile &= ~TILE_FLOODFILL;
}

// exsi bit == 001 alors == solide
// si bit == 011 alors ==vide etc
// tile c'est le bit
// if floodfill ou empty -> go
// if solid and not door-> go

void	flood_fill(t_game *game, t_vi2d pos, bool *is_open)
{
	t_tile	tile;

	tile = get_tile(&game->map, pos);
	if (tile & TILE_FLOODFILL)
		return ;
	if (tile & TILE_EMPTY)
	{
		*is_open = true;
		return ;
	}
	if ((tile & TILE_SOLID) && !(tile & TILE_DOOR))
		return ;
	set_tile(&game->map, pos, tile | TILE_FLOODFILL);
	flood_fill(game, vi2d(pos.x + 1, pos.y), is_open);
	flood_fill(game, vi2d(pos.x - 1, pos.y), is_open);
	flood_fill(game, vi2d(pos.x, pos.y + 1), is_open);
	flood_fill(game, vi2d(pos.x, pos.y - 1), is_open);
}

bool	is_map_open(t_game *game)
{
	bool	map_open;

	map_open = false;
	flood_fill(game, vi2d(game->pos.x, game->pos.y), &map_open);
	vec_tile_iter(&game->map.inner, tile_remove_floodfill_bit, NULL);
	return (map_open);
}
