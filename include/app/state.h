/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 14:01:08 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
#define STATE_H

#include "app/maps.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/types.h"
#include "me/vec2/vec2.h"

void cube_error(t_const_str fmt, ...);

typedef struct s_game t_game;

struct s_game
{
	t_vf2d			   pos;
	t_vf2d			   new_pos;
	t_f64			   angle;
	t_map			   map;
	t_hashmap_texture *textures;
};

t_error parse_map(t_game *game, t_const_str filename);
t_error fetch_textures(t_game *game);
void	init_game(t_game *game);

#endif /* STATE_H */
