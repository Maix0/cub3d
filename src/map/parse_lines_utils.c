/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:43:30 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:44:56 by maiboyer         ###   ########.fr       */
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

t_const_str	tex_name(t_texture tex)
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

t_const_str	col_name(t_u8 col)
{
	if (col == FLOOR_COLOR)
		return ("FLOOR");
	if (col == CEIL__COLOR)
		return ("CEILING");
	return ("UNKNOWN");
}

t_f64	_get_angle(char tile)
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

bool	map_contains_all_metadata(t_game *game)
{
	t_texture	tex;

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
