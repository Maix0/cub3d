/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 22:04:41 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
#define STATE_H

#define FOV 90
#define NUM_RAYS ctx->app.size_x
#include "app/maps.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/types.h"
#include "me/vec2/vec2.h"

typedef struct s_game t_game;
typedef struct s_ray  t_ray;

struct s_game
{
	t_vf2d			   pos; // x et y
	t_vf2d			   new_pos;
	t_f64			   angle; // direction
	t_map			   map;
	t_hashmap_texture *textures;
};

struct s_ray
{
	double ray_len;
	int	   sud;
	int	   est;
	int	   nord;
	int	   west;
	bool   hit_wall;
	double x;
	double y;
	double direction; // en degre
	t_tile tile;
};

t_error fetch_textures(t_blx *game);
t_error parse_map(t_game *game, t_const_str filename);
void	init_game(t_game *game);
void	cube_error(t_const_str fmt, ...);

#endif /* STATE_H */
