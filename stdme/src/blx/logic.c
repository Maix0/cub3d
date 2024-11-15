/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 18:06:06 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/15 14:28:19 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/blx/inputs.h"
#include "me/blx/sprite.h"
#include "mlx.h"
#include "sys/time.h"
#include <stdio.h>
#include <stdlib.h>

void _blx_get_mouse_pos(t_blx *blx);

static void blx_stop(t_blx *app)
{
	mlx_loop_end(app->mlx);
	blx_free(*app);
	exit(0);
}

// TODO: implement mouse buttons when needed
void blx_post_func(t_blx *ctx)
{
	t_usize idx;

	idx = 0;
	while (idx < ctx->inputs.keysyms_pressed.len && idx < ctx->inputs.keysyms_held.len)
	{
		ctx->inputs.keysyms_held.buffer[idx] |= ctx->inputs.keysyms_pressed.buffer[idx];
		ctx->inputs.keysyms_pressed.buffer[idx] = 0;
		idx++;
	}
	idx = 0;
	while (idx < ctx->inputs.keysyms_pressed.len)
		ctx->inputs.keysyms_pressed.buffer[idx++] = 0;
	idx = 0;
	while (idx < ctx->inputs.keysyms_released.len)
		ctx->inputs.keysyms_released.buffer[idx++] = 0;
}

int blx_loop_func(t_blx *ctx)
{
	struct timeval now;
	struct timeval diff;

	gettimeofday(&now, NULL);
	if (now.tv_usec < ctx->_data.timer.tv_usec)
	{
		diff.tv_sec = now.tv_sec - ctx->_data.timer.tv_sec - 1;
		diff.tv_usec = now.tv_usec - ctx->_data.timer.tv_usec + 1000000;
	}
	else
	{
		diff.tv_sec = now.tv_sec - ctx->_data.timer.tv_sec;
		diff.tv_usec = now.tv_usec - ctx->_data.timer.tv_usec;
	}
	_blx_get_mouse_pos(ctx);
	ctx->elapsed = ((t_f64)diff.tv_sec) + ((t_f64)diff.tv_usec / 1000000.0);
	ctx->_data.timer = now;
	mlx_put_image_to_window(ctx->mlx, ctx->win, ctx->_data.screen.img, 0, 0);
	if (ctx->_data.exit || ctx->func(ctx))
	{
		blx_stop(ctx);
		return (0);
	}
	blx_post_func(ctx);
	return (0);
}
