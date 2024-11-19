/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:05:08 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:03:40 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/blx/inputs.h"
#include "me/vec2/vec2.h"
#include <mlx.h>

void	_blx_get_mouse_pos(t_blx *blx)
{
	blx->inputs.mouse_old_pos = blx->inputs.mouse_pos;
	mlx_mouse_get_pos(blx->mlx, blx->win, &blx->inputs.mouse_pos.x, \
						&blx->inputs.mouse_pos.y);
}

void	blx_set_mouse_pos(t_blx *blx, t_vi2d pos)
{
	blx->inputs.mouse_pos = pos;
	blx->inputs.mouse_old_pos = pos;
	mlx_mouse_move(blx->mlx, blx->win, pos.x, pos.y);
}

t_vi2d	blx_get_mouse_delta(t_blx *blx)
{
	return (vi2d_sub(blx->inputs.mouse_pos, blx->inputs.mouse_old_pos));
}
