/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/18 22:04:04 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
#define STATE_H

#define FOV (PI / 4.0)

#define INC 0.2

#define NUM_RAYS ctx->app.size_x
#include "app/maps.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/types.h"
#include "me/vec2/vec2.h"

#ifndef BONUS
# define BONUS 0
#endif

typedef struct s_game t_game;
typedef struct s_ray  t_ray;

struct s_game
{
	bool			   exit;
	t_vf2d			   pos; // x et y
	t_vf2d			   new_pos;
	t_f64			   angle; // direction
	t_map			   map;
	t_hashmap_texture *textures;
	t_f64			   speed;
	t_f64			   rotate_speed;
	t_f64			   timer;
	bool			   mouse_enable;
};

struct s_ray
{
	double	  ray_len;
	t_texture tex;
	bool	  hit_wall;
	double	  x;
	double	  y;
	double	  direction; // en degre
	t_tile	  tile;
	double	  percent_wall;
};

t_error fetch_textures(t_blx *game);
t_error parse_map(t_game *game, t_const_str filename);
void	init_game(t_game *game);
void	cube_error(t_const_str fmt, ...);
bool	handle_input(t_blx *ctx, t_game *game);
void	draw_minimap(t_blx *ctx, t_game *game);

#endif /* STATE_H */
