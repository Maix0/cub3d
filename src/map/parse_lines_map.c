/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines_map.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:19:54 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 18:04:33 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/textures.h"
#include "me/char/char.h"
#include "me/convert/str_to_numbers.h"
#include "me/fs/fs.h"
#include "me/hashmap/hashmap_texture_path.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec/vec_str.h"

#define MAP_CHARSET "01 NSWED"

bool		is_map_open(t_game *game);
t_const_str	tex_name(t_texture tex);
t_const_str	col_name(t_u8 col);
t_f64		_get_angle(char tile);
t_error		read_whole_map(t_fd *file, t_vec_str *out);
bool		map_contains_all_metadata(t_game *game);
t_error		_extract_color(t_map_info *info, t_u8 col_type, t_str *line);

t_error	parse_map_handle_tile(\
			t_game *game, char tile, t_vf2d pos, t_usize *sp_count)
{
	if (tile == '0' || tile == '_')
		vec_tile_push(&game->map.inner, TILE_FLOOR);
	else if (tile == '1')
		vec_tile_push(&game->map.inner, TILE_WALL);
	else if (BONUS && tile == 'D')
		vec_tile_push(&game->map.inner, TILE_DOOR | TILE_SOLID);
	else if (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
	{
		(*sp_count)++;
		game->pos = pos;
		game->angle = _get_angle(tile);
		vec_tile_push(&game->map.inner, TILE_FLOOR);
	}
	else
		return (cube_error("Invalid map char: '%c'(%#02x)", tile, tile), ERROR);
	if (game->map.inner.buffer == NULL)
		return (cube_error("Alloc fail"), ERROR);
	return (NO_ERROR);
}

t_usize	_parse_map_set_vars(\
			t_game *game, t_vec_str *lines, t_usize map_start, t_usize *y)
{
	t_usize	max_width;

	*y = map_start;
	max_width = 0;
	while (*y < lines->len)
	{
		if (str_len(lines->buffer[*y]) > max_width)
			max_width = str_len(lines->buffer[*y]);
		(*y)++;
	}
	game->map.size.x = max_width;
	game->map.size.y = lines->len - map_start;
	*y = map_start;
	return (max_width);
}
