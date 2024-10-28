/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:56:05 by lgasqui           #+#    #+#             */
/*   Updated: 2024/10/28 12:40:08 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/fs/fs.h"
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec/vec_str.h"
#include "me/vec2/vec2.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_str get_textures_path(t_const_str line)
{
	t_i32 i;
	t_str path;

	i = 0;
	while (line[i] != '.')
		i++;
	path = str_clone(line + i);
	return (path);
}

t_error parse_line(t_map *data, t_str line)
{
	if (!line || !*line || isempty(line) || map_begin(line))
		return (NO_ERROR);
	else if (line[0] == 'N' && line[1] && line[1] == 'O')
		data->state.no_text = get_textures_path(line);
	else if (line[0] == 'S' && line[1] && line[1] == 'O')
		data->state.so_text = get_textures_path(line);
	else if (line[0] == 'W' && line[1] && line[1] == 'E')
		data->state.we_text = get_textures_path(line);
	else if (line[0] == 'E' && line[1] && line[1] == 'A')
		data->state.ea_text = get_textures_path(line);
	else if (line[0] == 'F' && line[1] && line[1] == ' ')
		get_bg_colors(line, &data->state.floor_colors);
	else if (line[0] == 'C' && line[1] && line[1] == ' ')
		get_bg_colors(line, &data->state.ceiling_colors);
	else
		return (cube_error("Invalid line in file"));
	return (NO_ERROR);
}

// Get the map in a file .cub and put it in a char**
t_error get_map(t_fd *fd, t_vec_str *out)
{
	t_string  line;
	t_vec_str map;

	map = vec_str_new(16, str_free);
	while (!getline_fd(fd, &line))
	{
		if (line.len != 0 && line.buf[line.len - 1] == '\n')
			string_pop(&line);
		vec_str_push(&map, line.buf);
	}
	return (*out = map, NO_ERROR);
}

t_error read_map(t_map *data, t_str filename, t_blx *blx)
{
	t_fd	 *fd;
	t_vec_str map;
	t_usize	  i;

	if (data == NULL || filename == NULL || blx == NULL)
		return (ERROR);
	printf("map\n");
	fd = open_fd(filename, FD_READ, 0, 0);
	if (fd == NULL)
		return (cube_error("Unable to open map file: %s", strerror(errno)), ERROR);
	if (get_map(fd, &map))
		return (ERROR);
	i = 0;
	printf("map.len = %zu\n", map.len);
	while (i < map.len)
	{
		printf("line[%zu] = '%s'\n", i, map.buffer[i]);
		if (parse_line(data, map.buffer[i++]))
			return (vec_str_free(map), cube_error("Unable to parse map"));
	}
	if (check_error(data, &map, blx))
		return (cube_error("Invalid map"));
	return (close_fd(fd), data->map = map, NO_ERROR);
}
