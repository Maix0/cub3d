/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:12:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 21:59:41 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPS_H
#define MAPS_H

#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/fs/fs.h"
#include "me/types.h"
#include "me/vec/vec_str.h"
#include "me/vec2/vec2.h"

typedef struct s_map	   t_map;
typedef struct s_map_state t_map_state;

struct s_map_state
{
	t_sprite no;
	t_sprite so;
	t_sprite we;
	t_sprite ea;
	t_str	 no_text;
	t_str	 so_text;
	t_str	 we_text;
	t_str	 ea_text;
	t_color	 floor_colors;
	t_color	 ceiling_colors;
};

struct s_map
{
	t_vec_str	map;
	t_vi2d		size;
	t_map_state state;
};

t_tile get_tile(t_map *map, t_vi2d p);
void   set_tile(t_map *map, t_vi2d p, t_tile tile);

// TRUC DE LOUVE //
bool	isempty(t_str line);
bool	map_begin(t_const_str line);
bool	map_charset(char c);
t_error check_error(t_map *data, t_vec_str *map, t_blx *blx);
t_error check_textures(t_map *data, t_blx *blx);
t_error get_bg_colors(t_const_str line, t_color *out);
t_error get_map(t_fd *fd, t_vec_str *out);
t_error map_format(t_str arg);
t_error parse_line(t_map *data, t_str line);
t_error valid_map(t_vec_str *map);
t_error read_map(t_map *data, t_str filename, t_blx *blx);
t_str	get_textures_path(t_const_str line);

#endif /* MAPS_H */
