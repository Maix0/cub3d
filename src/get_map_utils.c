/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:55:30 by lgasqui           #+#    #+#             */
/*   Updated: 2024/10/28 12:44:18 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/convert/str_to_numbers.h"
#include "me/mem/mem.h"
#include "me/printf/printf.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec2/vec2.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

bool map_charset(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

bool map_begin(t_const_str line)
{
	if (line[0] == '\n')
		return (false);
	while (*line && *line != '\n')
	{
		if (*line != ' ' && (*line < '0' || *line > '9'))
			return (false);
		line++;
	}
	return (true);
}

t_error get_bg_colors(t_const_str line, t_color *out)
{
	t_usize	  i;
	t_color	  res;
	t_vec_str colors;

	i = 0;
	while (line[i] && !(line[i] >= '0' && line[i] <= '9'))
		i++;
	if (str_split(&line[i], ", \n", &colors))
		return (ERROR);
	if (colors.len != 3)
		return (vec_str_free(colors), ERROR);
	if (str_to_u8(colors.buffer[0], 10, &res.r) ||
		str_to_u8(colors.buffer[1], 10, &res.g) ||
		str_to_u8(colors.buffer[2], 10, &res.b))

		return (ERROR);
	vec_str_free(colors);
	return (*out = res, NO_ERROR);
}
