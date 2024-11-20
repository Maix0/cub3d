/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:12:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/20 13:13:33 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPS_H
# define MAPS_H

# include "app/tile.h"
# include "me/blx/blx.h"
# include "me/fs/fs.h"
# include "me/hashmap/hashmap_texture_path.h"
# include "me/types.h"
# include "me/vec/vec_str.h"
# include "me/vec/vec_tile.h"
# include "me/vec2/vec2.h"

typedef struct s_map		t_map;
typedef struct s_map_info	t_map_info;

# define FLOOR_COLOR 0b0000010
# define CEIL__COLOR 0b0000001

struct s_map_info
{
	t_color					ceiling_color;
	t_color					floor_color;
	t_hashmap_texture_path	*textures_path;
	t_u8					color_bitfield;
};

struct s_map
{
	t_map_info	info;
	t_vec_tile	inner;
	t_vi2d		size;
};

t_tile	get_tile(t_map *map, t_vi2d p);
void	set_tile(t_map *map, t_vi2d p, t_tile tile);

#endif /* MAPS_H */
