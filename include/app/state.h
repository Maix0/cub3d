/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:11:18 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 13:20:46 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
#define STATE_H

#include "app/maps.h"
#include "me/types.h"
#include "me/vec2/vec2.h"

typedef struct s_game t_game;
struct s_game
{
	t_vf2d pos;
	t_f64  angle;
	t_map  map;
};

#endif /* STATE_H */
