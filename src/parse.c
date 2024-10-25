/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:48:00 by lgasqui           #+#    #+#             */
/*   Updated: 2024/10/25 22:00:57 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec2/vec2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

t_error valid_map(t_vec_str *map)
{
	t_usize x;
	t_usize y;

	y = 0;
	while (y < map->len)
	{
		x = 0;
		while (map->buffer[y][x])
		{
			if ((y == 0 && map->buffer[y][x] != '1' && map->buffer[y][x] != ' ') ||
				(x == 0 && map->buffer[y][x] != '1' && map->buffer[y][x] != ' ') ||
				(!map->buffer[y + 1] && map->buffer[y][x] != ' ' && map->buffer[y][x] != '1') ||
				(!map->buffer[y][x + 1] && map->buffer[y][x] != ' ' && map->buffer[y][x] != '1'))
				return (cube_error("Invalid Map 1"));
			else if (map->buffer[y][x] == '0' &&
					 (y && map->buffer[y + 1] && x && map->buffer[y][x + 1]) &&
					 (!map_charset(map->buffer[y + 1][x]) || !map_charset(map->buffer[y - 1][x]) ||
					  !map_charset(map->buffer[y][x + 1]) || !map_charset(map->buffer[y][x - 1])))
				return (cube_error("Invalid Map 2"));
			x++;
		}
		y++;
	}
	return (NO_ERROR);
}

bool isempty(t_str line)
{
	t_usize i;

	i = 0;
	while (line[i] && ((line[i] >= 9 && line[i] <= 13 && line[i] != '\n') || line[i] == ' '))
		i++;
	return (line[i] == '\n' || line[i] == '\0');
}

t_error map_format(t_str arg)
{
	t_usize i;

	i = str_len(arg) - 1;
	while (arg[i] && arg[i] != '.')
		i--;
	i++;
	if (arg[i] && arg[i] == 'c' && arg[i + 1] && arg[i + 1] == 'u' && arg[i + 2] &&
		arg[i + 2] == 'b' && !arg[i + 3])
		return (NO_ERROR);
	return (cube_error("Map is not a .cub !"));
}

t_error check_error(t_map *data, t_vec_str *map, t_blx *blx)
{
	if (map == NULL || check_textures(data, blx) || valid_map(map))
		return (ERROR);
	return (NO_ERROR);
}

// did the img load ? texture exist ?
t_error check_textures(t_map *data, t_blx *blx)
{
	t_usize check;

	if (!data->state.no_text || !data->state.so_text || !data->state.we_text ||
		!data->state.ea_text)
		return (cube_error("Invalid texture path"));
	check = 0;
	check += blx_sprite_from_xpm(blx, data->state.no_text, &data->state.no);
	check += blx_sprite_from_xpm(blx, data->state.so_text, &data->state.so);
	check += blx_sprite_from_xpm(blx, data->state.we_text, &data->state.we);
	check += blx_sprite_from_xpm(blx, data->state.ea_text, &data->state.ea);
	if (check)
		return (cube_error("Unable to open all textures files"));
	return (NO_ERROR);
}
