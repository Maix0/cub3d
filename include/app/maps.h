/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:12:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 13:20:00 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPS_H
#define MAPS_H

#include "me/types.h"
#include "me/vec2/vec2.h"

typedef enum e_tile	 t_tile;
typedef struct s_map t_map;

enum e_tile
{
	TILE_EMPTY = 0,
	TILE_SOLID = 1,

	TILE_FLOOR = 1 << 16 | ~TILE_SOLID,
	TILE_WALL  = 1 << 16 | TILE_SOLID,
};

struct s_map
{
	t_vi2d size;
};

t_tile get_tile(t_map *map, t_vi2d p);
void set_tile(t_map *map, t_vi2d p, t_tile tile);

#endif /* MAPS_H */
