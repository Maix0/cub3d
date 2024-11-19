/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:14:23 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:28:29 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/blx/blx.h"

#define THRESHOLD 2

void	handle_mouse_movement(t_blx *ctx, t_game *game)
{
	t_vi2d	movement;

	if (!BONUS)
		return ;
	if (!game->mouse_enable)
		return ;
	movement = blx_get_mouse_delta(ctx);
	if (movement.x < -THRESHOLD)
		game->angle -= PI / 32.0;
	else if (movement.x > THRESHOLD)
		game->angle += PI / 32.0;
	if (game->timer >= 0.25)
	{
		blx_set_mouse_pos(ctx, vi2d(ctx->app.size_x / 2, ctx->app.size_y / 2));
		game->timer = 0.0;
	}
	game->timer += ctx->elapsed;
}
