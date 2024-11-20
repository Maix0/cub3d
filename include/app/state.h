/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/20 13:12:56 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

# define INC 0.2
# define MAX_DIST 500.0

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
	bool				exit;
	bool				mouse_enable;
	double				fov;
	t_f64				angle;
	t_f64				rotate_speed;
	t_f64				speed;
	t_f64				timer;
	t_hashmap_texture	*textures;
	t_map				map;
	t_string			str;
	t_vf2d				new_pos;
	t_vf2d				pos;
};

struct					s_ray
{
	bool				hit_wall;
	double				direction;
	double				percent_wall;
	double				ray_len;
	double				x;
	double				y;
	t_texture			tex;
	t_tile				tile;
	t_vi2d				tile_pos;
};

bool					handle_input(t_blx *ctx, t_game *game);
t_error					fetch_textures(t_blx *game);
t_error					init_game(t_game *game);
t_error					parse_map(t_game *game, t_const_str filename);
void					cube_error(t_const_str fmt, ...);
void					draw_minimap(t_blx *ctx, t_game *game);

#endif /* STATE_H */
