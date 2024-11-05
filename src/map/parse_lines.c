/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:19:54 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/05 12:19:31 by maiboyer         ###   ########.fr       */
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
		return "EAST";
	if (tex == TEX_WEST)
		return "WEST";
	if (tex == TEX_NORTH)
		return "NORTH";
	if (tex == TEX_SOUTH)
		return "SOUTH";
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

t_error _extract_color(t_map_info *info, t_u8 col_type, t_str *line)
{
	t_color	 *slot;
	t_string  s;
	t_vec_str split;

	if (info->color_bitfield & col_type)
		return (cube_error("Duplicate color declaration: %s", col_name(col_type)), ERROR);
	// this allow us to make use of the same logic and just say "write to the color behind that
	// pointer"
	slot = &info->floor_color;
	if (col_type == CEIL__COLOR)
		slot = &info->ceiling_color;

	s = string_new(16);
	string_push(&s, *line);
	// remove the first char (C/F)
	string_remove(&s, 0, NULL);

	// we remove the spaces/tabs/whitespace that is before and after the filename.
	while (s.len > 0 && me_isspace(s.buf[0]))
		string_remove(&s, 0, NULL);
	while (s.len > 0 && me_isspace(s.buf[s.len - 1]))
		string_pop(&s);
	// split on whitespaces, as usual
	if (str_split(s.buf, ", \t\r", &split))
		return (string_free(s), ERROR);

	// this could be in its own function and just return error/whatever since print the same message
	// in case of error
	// whatever when doing the norminette pass
	if (s.len != 3)
		return (cube_error("Invalid format for color: %s", s.buf), string_free(s),
				vec_str_free(split), ERROR);
	if (str_to_u8(split.buffer[0], 10, &slot->r))
		return (cube_error("Invalid format for color: %s", s.buf), string_free(s),
				vec_str_free(split), ERROR);
	if (str_to_u8(split.buffer[1], 10, &slot->g))
		return (cube_error("Invalid format for color: %s", s.buf), string_free(s),
				vec_str_free(split), ERROR);
	if (str_to_u8(split.buffer[2], 10, &slot->b))
		return (cube_error("Invalid format for color: %s", s.buf), string_free(s),
				vec_str_free(split), ERROR);
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

	// we remove the spaces/tabs/whitespace that is before and after the filename.
	// yes this means that this line:
	// NO ./banane/      truc.xpm
	// is a valid path, but we can't have files that ends in whitespaces, since they would get
	// removed
	while (s.len > 0 && me_isspace(s.buf[0]))
		string_remove(&s, 0, NULL);
	while (s.len > 0 && me_isspace(s.buf[s.len - 1]))
		string_pop(&s);

	// if the hmap_<name>_insert returns true, this means that the element was already in the hmap,
	// thus meaning that we have a duplicate texture definition

	if (hmap_texture_path_insert(info->textures_path, tex, s))
		return (cube_error("Duplicate texture declaration: %s", tex_name(tex)), ERROR);
	return (NO_ERROR);
}

t_error get_mapinfo(t_vec_str *lines, t_map_info *out, t_usize *mstart_idx)
{
	(void)(lines);
	(void)(out);
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
		if (lines->buffer[i][0] != '\0' && str_is_charset(lines->buffer[i], MAP_CHARSET))
			return (*mstart_idx = i, NO_ERROR);
		i++;
	}
	// we arrived here without any map, just information about the textures/colors
	return (cube_error("No map data found"), ERROR);
}
