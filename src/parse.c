/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:48:00 by lgasqui           #+#    #+#             */
/*   Updated: 2024/10/25 14:58:57 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/string/string.h"
#include "me/vec2/vec2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "me/str/str.h"

int	valid_map(char **map)
{
	int	x;
	int	y;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if ((y == 0 && map[y][x] != '1' && map[y][x] != ' ')
				|| (x == 0 && map[y][x] != '1' && map[y][x] != ' ')
				|| (!map[y + 1] && map[y][x] != ' ' && map[y][x] != '1')
				|| (!map[y][x + 1] && map[y][x] != ' ' && map[y][x] != '1'))
				return (printf("Error.\nInvalid map.\n"), 0);
			else if (map[y][x] == '0'
				&& (y && map[y + 1] && x && map[y][x + 1])
				&& (!map_charset(map[y + 1][x]) || !map_charset(map[y - 1][x])
				|| !map_charset(map[y][x + 1]) || !map_charset(map[y][x - 1])))
				return (printf("Error.\nInvalid map.\n"), 0);
			x++;
		}
		y++;
	}
	return (1);
}

int	isempty(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ((line[i] >= 9 && line[i] <= 13 && line[i] != '\n')
			|| line[i] == ' '))
		i++;
	return (line[i] == '\n');
}

int	map_format(char *arg)
{
	int	i;

	i = str_len(arg) - 1;
	while (arg[i] && arg[i] != '.')
		i--;
	i++;
	if (arg[i] && arg[i] == 'c' && arg[i + 1] && arg[i + 1] == 'u' && arg[i + 2]
		&& arg[i + 2] == 'b' && !arg[i + 3])
		return (1);
	return (printf("Error.\nMap is not a .cub\n"), 0);
}

int	check_error(t_data *data, char **map, t_blx *blx)
{
	if (!check_textures(data, blx) || !map || !valid_map(map))
		return (0);
	return (1);
}

//did the img load ? texture exist ?
int	check_textures(t_data *data, t_blx *blx)
{
	int	check;

	if (!data->no_text || !data->so_text
		|| !data->we_text || !data->ea_text)
		return (printf("Error.\nInvalid texture path1.\n"), 0);
	check = 0;
	check += blx_sprite_from_xpm(blx, data->no_text, &data->no);
	check += blx_sprite_from_xpm(blx, data->so_text, &data->so);
	check += blx_sprite_from_xpm(blx, data->we_text, &data->we);
	check += blx_sprite_from_xpm(blx, data->ea_text, &data->ea);
	if (check)
		return (printf("Error.\nInvalid texture path2.\n"), 0);
	return (1);
}