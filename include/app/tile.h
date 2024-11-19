/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:36:32 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 16:43:29 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TILE_H
# define TILE_H

# include "me/types.h"
# include "me/vec2/vec2.h"

typedef enum e_tile	t_tile;
enum e_tile
{
	TILE_EMPTY = 1 << 1,
	TILE_SOLID = 1 << 2,
	TILE_DOOR = 1 << 3,
	TILE_FLOODFILL = 1 << 4,
	TILE_FLOOR = 1 << 16,
	TILE_WALL = 1 << 16 | TILE_SOLID,
};

#endif /* TILE_H */
