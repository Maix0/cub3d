/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 12:42:52 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
#define STATE_H

# define FOV 90
#include "app/maps.h"
#include "me/types.h"
#include "me/vec2/vec2.h"

t_error cube_error(t_const_str fmt, ...);

typedef struct s_game t_game;
typedef struct s_ray t_ray;

struct s_game
{
	t_vf2d pos; //x et y
	t_vf2d new_pos; 
	t_f64  angle; // direction
	t_map  map;
};

struct s_ray
{
	double ray_len;
	int		sud;
	int		est;
	int		nord;
	int		west;
	bool	hit_wall;
	double	angle; // en degre
		
};

#endif /* STATE_H */
