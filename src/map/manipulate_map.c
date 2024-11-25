/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:31:46 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/25 13:13:38 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/tile.h"
#include "me/types.h"
#include "me/vec/vec_tile.h"
#include "me/vec2/vec2.h"

//recup parsing map avant le set up des bits
t_tile	get_tile(t_map *map, t_vi2d pos)
{
	t_tile	*out;

	if (!(0 <= pos.y && pos.y < map->size.y && 0 <= pos.x
			&& pos.x < map->size.x))
		return (TILE_EMPTY | TILE_SOLID);
	out = vec_tile_get(&map->inner, (t_usize)(pos.y * map->size.x + pos.x));
	if (out == NULL)
		return (TILE_EMPTY | TILE_SOLID);
	return (*out);
}

void	set_tile(t_map *map, t_vi2d pos, t_tile tile)
{
	t_tile	*ptr;

	if (!(0 <= pos.y && pos.y < map->size.y && 0 <= pos.x
			&& pos.x < map->size.x))
		return ;
	ptr = vec_tile_get(&map->inner, (t_usize)(pos.y * map->size.x + pos.x));
	if (ptr == NULL)
		return ;
	*ptr = tile;
}
