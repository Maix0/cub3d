/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:12:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 14:54:41 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAPS_H
#define MAPS_H

#include "me/types.h"
#include "me/vec2/vec2.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/fs/fs.h"
#include "me/vec/vec_str.h"

typedef struct s_map t_map;

struct s_map
{
	t_vi2d size;
};

typedef struct s_data
{
	char		**map;
	int			map_h;
	int			map_w;
	t_sprite	no;
	t_sprite	so;
	t_sprite	we;
	t_sprite	ea;
	t_str	no_text;
	t_str	so_text;
	t_str	we_text;
	t_str	ea_text;
	t_color		floor_colors;
	t_color		ceiling_colors;
	

}	t_data;
t_tile get_tile(t_map *map, t_vi2d p);
void set_tile(t_map *map, t_vi2d p, t_tile tile);

// TRUC DE LOUVE //
int	valid_map(char **map);
void	replace_nl(char	*str);
char	*get_textures_path(char *line);
int	parse_line(t_data *data, char *line);
t_error get_map(t_fd *fd, t_vec_str *out);
char	**read_map(t_data *data, char *filename, t_blx *blx);
int	map_charset(char c);
int	map_begin(char	*line);
int	ft_only_nb(char	**tab);
int	ft_tablen(char	**tab);
t_error get_bg_colors(char *line, t_color *out);
int	map_format(char *arg);
int	check_error(t_data *data, char **map, t_blx *blx);
int	check_colors(t_data *data);
int	check_textures(t_data *data, t_blx *blx);
int	isempty(char *line);

#endif /* MAPS_H */
