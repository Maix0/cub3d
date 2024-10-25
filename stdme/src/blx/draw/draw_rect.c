/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_rect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:56:25 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/24 17:12:03 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/vec2/vec2.h"

void blx_draw_rect(t_blx *ctx, t_vi2d p1, t_vi2d p2, t_color col)
{
	blx_draw_line(ctx, p1, vi2d(p1.x, p2.y), col);
	blx_draw_line(ctx, p1, vi2d(p2.x, p1.y), col);
	blx_draw_line(ctx, p2, vi2d(p1.x, p2.y), col);
	blx_draw_line(ctx, p2, vi2d(p2.x, p1.y), col);
}
void blx_fill_rect(t_blx *ctx, t_vi2d p1, t_vi2d p2, t_color col)
{
	t_vi2d tmp;
	t_i64  y;

	if (p1.y > p2.y)
	{
		tmp = p1;
		p1 = p2;
		p2 = tmp;
	}
	y = p1.y;
	while (y <= p2.y)
	{
		blx_draw_line(ctx, vi2d(p1.x, y), vi2d(p2.x, y), col);
		y++;
	}
}
