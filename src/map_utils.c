/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 12:31:46 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/30 12:33:39 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/tile.h"
#include "me/types.h"
#include "me/vec/vec_tile.h"
#include "me/vec2/vec2.h"

t_tile get_tile(t_map *map, t_vi2d pos)
{
	t_tile *out;

	out = vec_tile_get(&map->map, (t_usize)(pos.y * map->size.x + pos.x));
	if (out == NULL)
		return (TILE_EMPTY);
	return (*out);
}
