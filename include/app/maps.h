/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:12:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 13:37:36 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPS_H
#define MAPS_H

#include "me/types.h"
#include "me/vec2/vec2.h"
#include "app/tile.h"

typedef struct s_map t_map;

struct s_map
{
	t_vi2d size;
};

t_tile get_tile(t_map *map, t_vi2d p);
void set_tile(t_map *map, t_vi2d p, t_tile tile);

#endif /* MAPS_H */
