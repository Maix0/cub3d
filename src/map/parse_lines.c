/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:19:54 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 18:05:17 by maiboyer         ###   ########.fr       */
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
t_error		parse_map_handle_tile(\
			t_game *game, char tile, t_vf2d pos, t_usize *sp_count);
t_usize		_parse_map_set_vars(\
			t_game *game, t_vec_str *lines, t_usize map_start, t_usize *y);

t_error	_mapinfo_extract_path(t_map_info *info, t_texture tex, t_str *line)
{
	t_string	s;

	s = string_new(16);
	string_push(&s, *line);
	string_remove(&s, 0, NULL);
	string_remove(&s, 0, NULL);
	while (s.len > 0 && me_isspace(s.buf[0]))
		string_remove(&s, 0, NULL);
	while (s.len > 0 && me_isspace(s.buf[s.len - 1]))
		string_pop(&s);
	if (hmap_texture_path_insert(info->textures_path, tex, s))
		return (cube_error("Duplicate texture declaration: %s", \
				tex_name(tex)), ERROR);
	return (NO_ERROR);
}

t_error	_mapinfo_starts_with(t_vec_str *lines, t_usize i, t_map_info *out)
{
	return ((str_startwith(lines->buffer[i], "NO") && \
				_mapinfo_extract_path(out, TEX_NORTH, &lines->buffer[i])) || \
			(str_startwith(lines->buffer[i], "EA") && \
				_mapinfo_extract_path(out, TEX_EAST, &lines->buffer[i])) || \
			(str_startwith(lines->buffer[i], "SO") && \
				_mapinfo_extract_path(out, TEX_SOUTH, &lines->buffer[i])) || \
			(str_startwith(lines->buffer[i], "WE") && \
				_mapinfo_extract_path(out, TEX_WEST, &lines->buffer[i])));
}

t_error	get_mapinfo(t_vec_str *lines, t_map_info *out, t_usize *mstart_idx)
{
	t_usize	i;

	i = 0;
	while (i < lines->len)
	{
		if (_mapinfo_starts_with(lines, i, out))
			return (ERROR);
		if (str_startwith(lines->buffer[i], "F") && \
			_extract_color(out, FLOOR_COLOR, &lines->buffer[i]))
			return (ERROR);
		if (str_startwith(lines->buffer[i], "C") && \
			_extract_color(out, CEIL__COLOR, &lines->buffer[i]))
			return (ERROR);
		if (lines->buffer[i][0] != '\0' && \
			str_is_charset(lines->buffer[i], MAP_CHARSET))
			return (*mstart_idx = i, NO_ERROR);
		i++;
	}
	return (cube_error("No map data found"), ERROR);
}

t_error	parse_map_inner(t_game *game, t_vec_str lines, t_usize map_start)
{
	t_usize	y;
	t_usize	x;
	t_usize	sp_count;
	t_usize	max_width;

	max_width = _parse_map_set_vars(game, &lines, map_start, &y);
	sp_count = 0;
	while (y < lines.len)
	{
		x = 0;
		while (lines.buffer[y][x] != 0)
		{
			if (parse_map_handle_tile(game, lines.buffer[y][x], \
					vf2d(x + 0.5, y - map_start + 0.5), &sp_count))
				return (vec_str_free(lines), ERROR);
			x++;
		}
		while (x++ < max_width)
			vec_tile_push(&game->map.inner, TILE_EMPTY);
		y++;
	}
	if (vec_str_free(lines), sp_count != 1)
		return (cube_error("Invalid spawn count (%u)", sp_count), ERROR);
	return (NO_ERROR);
}

t_error	parse_map(t_game *game, t_const_str filename)
{
	t_fd		*file;
	t_vec_str	lines;
	t_usize		map_idx;

	if (game == NULL || filename == NULL)
		return (ERROR);
	if (!str_endwith(filename, ".cub"))
		return (cube_error("Map isn't .cub file"), ERROR);
	file = open_fd((t_str)filename, FD_READ, 0, 0);
	if (file == NULL)
		return (cube_error("Failed to open map file"), ERROR);
	if (read_whole_map(file, &lines))
		return (cube_error("Failed to read whole map"), ERROR);
	close_fd(file);
	if (get_mapinfo(&lines, &game->map.info, &map_idx))
		return (vec_str_free(lines), ERROR);
	if (parse_map_inner(game, lines, map_idx))
		return (ERROR);
	if (!map_contains_all_metadata(game))
		return (ERROR);
	if (is_map_open(game))
		return (cube_error("Map is open !"), ERROR);
	return (NO_ERROR);
}
