/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines_extract_color.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:19:54 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:50:31 by maiboyer         ###   ########.fr       */
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

t_error	_extract_color_inner(\
				t_map_info *info, t_u8 col_type, t_str *line, t_string *s)
{
	if (info->color_bitfield & col_type)
		return (cube_error("Duplicate color declaration: %s", \
		col_name(col_type)), ERROR);
	*s = string_new(16);
	string_push(s, *line);
	string_remove(s, 0, NULL);
	while (s->len > 0 && me_isspace(s->buf[0]))
		string_remove(s, 0, NULL);
	while (s->len > 0 && me_isspace(s->buf[s->len - 1]))
		string_pop(s);
	return (NO_ERROR);
}

t_error	_col_atoh(t_vec_str *split, t_color *slot)
{
	if (split->len != 3)
		return (ERROR);
	if (str_to_u8(split->buffer[0], 10, &slot->r))
		return (ERROR);
	if (str_to_u8(split->buffer[1], 10, &slot->g))
		return (ERROR);
	if (str_to_u8(split->buffer[2], 10, &slot->b))
		return (ERROR);
	return (NO_ERROR);
}

t_error	_extract_color(t_map_info *info, t_u8 col_type, t_str *line)
{
	t_color		*slot;
	t_string	s;
	t_vec_str	split;

	if (_extract_color_inner(info, col_type, line, &s))
		return (ERROR);
	if (str_split(s.buf, ", \t\r", &split))
		return (string_free(s), ERROR);
	slot = &info->floor_color;
	if (col_type == CEIL__COLOR)
		slot = &info->ceiling_color;
	if (_col_atoh(&split, slot))
		return (cube_error("Invalid format for color: %s", s.buf), \
				string_free(s), vec_str_free(split), ERROR);
	slot->a = 0x0;
	(string_free(s), vec_str_free(split));
	info->color_bitfield |= col_type;
	return (NO_ERROR);
}
