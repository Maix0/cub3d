/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:19:54 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 14:40:26 by maiboyer         ###   ########.fr       */
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

#define MAP_CHARSET "01 NSWE"

t_const_str tex_name(t_texture tex)
{
	if (tex == TEX_EAST)
		return ("EAST");
	if (tex == TEX_WEST)
		return ("WEST");
	if (tex == TEX_NORTH)
		return ("NORTH");
	if (tex == TEX_SOUTH)
		return ("SOUTH");
	return ("UNKNOWN");
}

t_const_str col_name(t_u8 col)
{
	if (col == FLOOR_COLOR)
		return ("FLOOR");
	if (col == CEIL__COLOR)
		return ("CEILING");
	return ("UNKNOWN");
}

t_f64 _get_angle(char tile)
{
	if (tile == 'E')
		return (0);
	if (tile == 'W')
		return (PI);
	if (tile == 'N')
		return (-PI / 2.0);
	if (tile == 'S')
		return (PI / 2.0);
	return (0);
}

t_error _extract_color(t_map_info *info, t_u8 col_type, t_str *line)
{
	t_color	 *slot;
	t_string  s;
	t_vec_str split;

	if (info->color_bitfield & col_type)
		return (
			cube_error("Duplicate color declaration: %s", col_name(col_type)),
			ERROR);
	// this allow us to make use of the same logic and just say "write to the
	// color behind that pointer"
	slot = &info->floor_color;
	if (col_type == CEIL__COLOR)
		slot = &info->ceiling_color;
	s = string_new(16);
	string_push(&s, *line);
	// remove the first char (C/F)
	string_remove(&s, 0, NULL);
	// we remove the spaces/tabs/whitespace that is before and after the
	// filename.
	while (s.len > 0 && me_isspace(s.buf[0]))
		string_remove(&s, 0, NULL);
	while (s.len > 0 && me_isspace(s.buf[s.len - 1]))
		string_pop(&s);
	// split on whitespaces, as usual
	if (str_split(s.buf, ", \t\r", &split))
		return (string_free(s), ERROR);
	// this could be in its own function and just return error/whatever since
	// print the same message in case of error whatever when doing the
	// norminette pass
	if (split.len != 3)
		return (cube_error("Invalid format for color: %s", s.buf),
				string_free(s), vec_str_free(split), ERROR);
	slot->a = 0x0;
	// Extract colors
	if (str_to_u8(split.buffer[0], 10, &slot->r))
		return (cube_error("Invalid format for color: %s", s.buf),
				string_free(s), vec_str_free(split), ERROR);
	if (str_to_u8(split.buffer[1], 10, &slot->g))
		return (cube_error("Invalid format for color: %s", s.buf),
				string_free(s), vec_str_free(split), ERROR);
	if (str_to_u8(split.buffer[2], 10, &slot->b))
		return (cube_error("Invalid format for color: %s", s.buf),
				string_free(s), vec_str_free(split), ERROR);
	(string_free(s), vec_str_free(split));
	info->color_bitfield |= col_type;
	return (NO_ERROR);
}

t_error _extract_path(t_map_info *info, t_texture tex, t_str *line)
{
	t_string s;

	s = string_new(16);
	string_push(&s, *line);
	// remove the first two char (NO, EA, SO, WE)
	string_remove(&s, 0, NULL);
	string_remove(&s, 0, NULL);
	// we remove the spaces/tabs/whitespace that is before and after the
	// filename. yes this means that this line: NO ./banane/      truc.xpm is a
	// valid path, but we can't have files that ends in whitespaces, since they
	// would get removed
	while (s.len > 0 && me_isspace(s.buf[0]))
		string_remove(&s, 0, NULL);
	while (s.len > 0 && me_isspace(s.buf[s.len - 1]))
		string_pop(&s);
	// if the hmap_<name>_insert returns true, this means that the element was
	// already in the hmap, thus meaning that we have a duplicate texture
	// definition
	if (hmap_texture_path_insert(info->textures_path, tex, s))
		return (cube_error("Duplicate texture declaration: %s", tex_name(tex)),
				ERROR);
	return (NO_ERROR);
}

t_error get_mapinfo(t_vec_str *lines, t_map_info *out, t_usize *mstart_idx)
{
	t_usize i;

	i = 0;
	while (i < lines->len)
	{
		if ((str_startwith(lines->buffer[i], "NO") &&
			 _extract_path(out, TEX_NORTH, &lines->buffer[i])) ||
			(str_startwith(lines->buffer[i], "EA") &&
			 _extract_path(out, TEX_EAST, &lines->buffer[i])) ||
			(str_startwith(lines->buffer[i], "SO") &&
			 _extract_path(out, TEX_SOUTH, &lines->buffer[i])) ||
			(str_startwith(lines->buffer[i], "WE") &&
			 _extract_path(out, TEX_WEST, &lines->buffer[i])))
			return (ERROR);
		if (str_startwith(lines->buffer[i], "F") &&
			_extract_color(out, FLOOR_COLOR, &lines->buffer[i]))
			return (ERROR);
		if (str_startwith(lines->buffer[i], "C") &&
			_extract_color(out, CEIL__COLOR, &lines->buffer[i]))
			return (ERROR);
		if (lines->buffer[i][0] != '\0' &&
			str_is_charset(lines->buffer[i], MAP_CHARSET))
			return (*mstart_idx = i, NO_ERROR);
		i++;
	}
	// we arrived here without any map, just information about the
	// textures/colors
	return (cube_error("No map data found"), ERROR);
}

t_error parse_map_inner(t_game *game, t_vec_str lines, t_usize map_start)
{
	t_usize y;
	t_usize x;
	t_usize max_width;
	t_usize sp_count;
	char	tile;

	y = map_start;
	max_width = 0;
	while (y < lines.len)
	{
		if (str_len(lines.buffer[y]) > max_width)
			max_width = str_len(lines.buffer[y]);
		y++;
	}
	game->map.size.x = max_width;
	game->map.size.y = lines.len - map_start;
	y = map_start;
	sp_count = 0;
	while (y < lines.len)
	{
		x = 0;
		while (lines.buffer[y][x] != 0)
		{
			tile = lines.buffer[y][x];
			if (tile == '0')
				vec_tile_push(&game->map.inner, TILE_FLOOR);
			else if (tile == '1')
				vec_tile_push(&game->map.inner, TILE_WALL);
			else if (tile == ' ')
				vec_tile_push(&game->map.inner, TILE_EMPTY | TILE_SOLID);
			else if (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
			{
				sp_count++;
				game->pos = vf2d(x + 0.5, y - map_start + 0.5);
				game->angle = _get_angle(tile);
				vec_tile_push(&game->map.inner, TILE_FLOOR);
			}
			else
				return (cube_error("Invalid map character: '%c'(%#02x)", tile,
								   tile),
						vec_str_free(lines), ERROR);
			x++;
		}
		while (x++ < max_width)
			vec_tile_push(&game->map.inner, TILE_EMPTY | TILE_SOLID);
		y++;
	}
	vec_str_free(lines);
	if (sp_count != 1)
		return (cube_error("Invalid spawn count (%u)", sp_count), ERROR);
	return (NO_ERROR);
}

t_error read_whole_map(t_fd *file, t_vec_str *out);

bool map_contains_all_metadata(t_game *game)
{
	t_texture tex;

	if (!(game->map.info.color_bitfield & FLOOR_COLOR))
		return (cube_error("Missing Floor color"), false);
	if (!(game->map.info.color_bitfield & CEIL__COLOR))
		return (cube_error("Missing Ceiling color"), false);
	tex = TEX_EAST;
	if (hmap_texture_path_get(game->map.info.textures_path, &tex) == NULL)
		return (cube_error("Missing East Texture"), false);
	tex = TEX_WEST;
	if (hmap_texture_path_get(game->map.info.textures_path, &tex) == NULL)
		return (cube_error("Missing West Texture"), false);
	tex = TEX_NORTH;
	if (hmap_texture_path_get(game->map.info.textures_path, &tex) == NULL)
		return (cube_error("Missing North Texture"), false);
	tex = TEX_SOUTH;
	if (hmap_texture_path_get(game->map.info.textures_path, &tex) == NULL)
		return (cube_error("Missing South Texture"), false);
	return (true);
}

t_error parse_map(t_game *game, t_const_str filename)
{
	t_fd	 *file;
	t_vec_str lines;
	t_usize	  map_idx;

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
		return (hmap_texture_path_free(game->map.info.textures_path),
				vec_str_free(lines), ERROR);
	if (parse_map_inner(game, lines, map_idx))
		return (ERROR);
	if (!map_contains_all_metadata(game))
		return (ERROR);
	return (NO_ERROR);
}
