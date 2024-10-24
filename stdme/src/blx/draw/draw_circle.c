/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_circle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:45 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/24 14:09:14 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/vec2/vec2.h"
#include <stdio.h>

static inline void _print_point(t_blx *app, t_vi2d center, t_vi2d pos, t_color col)
{
	blx_draw(app, vi2d(center.x + pos.x, center.y + pos.y), col);
	blx_draw(app, vi2d(center.x - pos.x, center.y + pos.y), col);
	blx_draw(app, vi2d(center.x + pos.x, center.y - pos.y), col);
	blx_draw(app, vi2d(center.x - pos.x, center.y - pos.y), col);
	blx_draw(app, vi2d(center.x + pos.y, center.y + pos.x), col);
	blx_draw(app, vi2d(center.x - pos.y, center.y + pos.x), col);
	blx_draw(app, vi2d(center.x + pos.y, center.y - pos.x), col);
	blx_draw(app, vi2d(center.x - pos.y, center.y - pos.x), col);
}

void blx_draw_circle(t_blx *app, t_vi2d center, t_i32 r, t_color col)
{
	t_vi2d pos;
	t_i32  p;

	pos = vi2d(0, -r);
	p = -r;
	while (pos.x < -pos.y)
	{
		if (p >= 0)
		{
			pos.y += 1;
			p += 2 * (pos.x + pos.y) + 1;
		}
		else
			p += 2 * pos.x + 1;
		_print_point(app, center, pos, col);
		pos.x += 1;
	}
}
