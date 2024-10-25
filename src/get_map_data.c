/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:56:05 by lgasqui           #+#    #+#             */
/*   Updated: 2024/10/25 15:07:30 by lgasqui          ###   ########.fr       */
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
#include "me/fs/fs.h"
#include "me/vec/vec_str.h"

void	replace_nl(char	*str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	str[i] = '\0';
}

char	*get_textures_path(char *line)
{
	int		i;
	char	*path;

	i = 0;
	while (line[i] != '.')
		i++;
	path = str_clone(line + i);
	replace_nl(path);
	return (path);
}

int	parse_line(t_data *data, char *line)
{
	if (!line || !*line || isempty(line) || map_begin(line))
		return (0);
	else if (line[0] == 'N' && line[1] && line[1] == 'O')
		data->no_text = get_textures_path(line);
	else if (line[0] == 'S' && line[1] && line[1] == 'O')
		data->so_text = get_textures_path(line);
	else if (line[0] == 'W' && line[1] && line[1] == 'E')
		data->we_text = get_textures_path(line);
	else if (line[0] == 'E' && line[1] && line[1] == 'A')
		data->ea_text = get_textures_path(line);
	else if (line[0] == 'F' && line[1] && line[1] == ' ')
		get_bg_colors(line, &data->floor_colors);
	else if (line[0] == 'C' && line[1] && line[1] == ' ')
		get_bg_colors(line, &data->ceiling_colors);
	else
		return (printf("Error.\nInvalid line in file.\n"), 1);
	return (0);
}

//Get the map in a file .cub and put it in a char**
t_error get_map(t_fd *fd, t_vec_str *out)
{
	t_string line;
	t_vec_str map;

	map = vec_str_new(16, str_free);
	while (!getline_fd(fd, &line))
		vec_str_push(&map, line.buf);
	return (*out = map, NO_ERROR);
}

char	**read_map(t_data *data, char *filename, t_blx *blx)
{
	t_fd *fd;
	t_vec_str map;
	unsigned int i;

	fd = open_fd(filename, FD_READ, 0, 0);
	if (fd == NULL)
		return (perror("map file"), NULL);
	// line = get_next_line(fd);
	// if (!line)
	// 	return (NULL);
	// if (parse_line(data, line))
	// 	return (get_next_line(-1), free(line), me_exit(1), NULL);
	// while (line && !map_begin(line))
	// {
	// 	free(line);
	// 	line = get_next_line(fd);
	// 	if (!line)
	// 		return (NULL);
	// 	if (parse_line(data, line))
	// 		return (get_next_line(-1), free(line), fire_exit(data, NULL), NULL);
	// }
	if (get_map(fd, &map)) 
	return (NULL);
	i = 0;
	printf("map.len = %zu\n", map.len);
	while(i < map.len)
	{
		printf("line[%i] = '%s'\n", i, map.buffer[i]);
	if (parse_line(data, map.buffer[i++]))
		return (vec_str_free(map), NULL);
	}
	if (!check_error(data, map.buffer, blx))
		return (exit(1), NULL);
	return (close_fd(fd), map.buffer);
}