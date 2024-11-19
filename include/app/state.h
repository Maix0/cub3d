/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 18:35:25 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

# define INC 0.2

# include "app/maps.h"
# include "me/hashmap/hashmap_texture.h"
# include "me/string/string.h"
# include "me/types.h"
# include "me/vec2/vec2.h"

# ifndef BONUS
#  define BONUS 0
# endif

typedef struct s_game	t_game;
typedef struct s_ray	t_ray;

struct					s_game
{
	double				fov;
	bool				exit;
	t_vf2d				pos;
	t_vf2d				new_pos;
	t_f64				angle;
	t_map				map;
	t_hashmap_texture	*textures;
	t_f64				speed;
	t_f64				rotate_speed;
	t_f64				timer;
	bool				mouse_enable;
	t_string			str;
};

struct					s_ray
{
	double				ray_len;
	t_texture			tex;
	bool				hit_wall;
	double				x;
	double				y;
	double				direction;
	t_tile				tile;
	double				percent_wall;
	t_vi2d				tile_pos;
};

bool					handle_input(t_blx *ctx, t_game *game);
t_error					fetch_textures(t_blx *game);
t_error					init_game(t_game *game);
t_error					parse_map(t_game *game, t_const_str filename);
void					cube_error(t_const_str fmt, ...);
void					draw_minimap(t_blx *ctx, t_game *game);

#endif /* STATE_H */
