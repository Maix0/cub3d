/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:55:30 by lgasqui           #+#    #+#             */
/*   Updated: 2024/10/25 15:08:17 by lgasqui          ###   ########.fr       */
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
#include "me/convert/str_to_numbers.h"

int	map_charset(char c)
{
	if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

int	map_begin(char	*line)
{
	if (line[0] == '\n')
		return (0);
	while (*line && *line != '\n')
	{
		if (*line != ' ' && (*line < '0' || *line > '9'))
			return (0);
		line++;
	}
	return (1);
}

int	ft_only_nb(char	**tab)
{
	int	i;
	int	j;

	i = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			if (tab[i][j] < '0' || tab[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_tablen(char	**tab)
{
	int	len;

	len = 0;
	while (tab[len])
		len++;
	return (len);
}

t_error get_bg_colors(char *line, t_color *out)
{
	int		i;
	t_color		res;
	char	*clear;
	t_vec_str	check;

	i = 0;
	while (line[i] && !(line[i] >= '0' && line[i] <= '9'))
		i++;
	clear = str_clone(line + i);
	if (!clear)
		return (ERROR);
	if (str_split(clear, ", \n", &check))
		return (ERROR);
	mem_free(clear);
	if (check.len != 3)
		return (vec_str_free(check), ERROR);
	//res = malloc(3 * sizeof(int));
	//if (!res)
	// 	return (NULL);
	// i = -1;
	// while (check.buffer[++i])
	// 	res[i] = ft_atoi(check.buffer[i]);
	if (str_to_u8(check.buffer[0], 10, &res.r) || str_to_u8(check.buffer[1], 10, &res.g) || str_to_u8(check.buffer[2], 10, &res.b))
		return (ERROR);
	return (vec_str_free(check), *out = res, ERROR);
}