/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:58:47 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/24 15:26:57 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"

struct s_draw_line_state
{
	t_vi2d d;
	t_i32  p;
	t_i32  idx;
	t_i32  dir;
};

t_i32 _abs_sub(t_i32 lhs, t_i32 rhs)
{
	t_i32 res;
	res = (lhs - rhs);
	if (res < 0)
		res = -res;
	return (res);
}
void _swap_vi2d(t_vi2d *lhs, t_vi2d *rhs)
{
	t_vi2d tmp;

	tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

#define PLOTLINE(name, major, minor)                                                               \
	void _line_##name(t_blx *ctx, t_vi2d p0, t_vi2d p1, t_color col)                               \
	{                                                                                              \
		t_vi2d d;                                                                                  \
		t_i32  D;                                                                                  \
		t_i32  x;                                                                                  \
		t_i32  y;                                                                                  \
		t_i32  major##i;                                                                           \
		d.x = p1.x - p0.x;                                                                         \
		d.y = p1.y - p0.y;                                                                         \
		major##i = 1;                                                                              \
		if (d.major < 0)                                                                           \
		{                                                                                          \
			major##i = -1;                                                                         \
			d.major = -d.major;                                                                    \
		}                                                                                          \
		D = (2 * d.major) - d.minor;                                                               \
		x = p0.x;                                                                                  \
		y = p0.y;                                                                                  \
		while (minor <= p1.minor)                                                                  \
		{                                                                                          \
			blx_draw(ctx, vi2d(x, y), col);                                                        \
			if (D > 0)                                                                             \
			{                                                                                      \
				major += major##i;                                                                 \
				D += 2 * (d.major - d.minor);                                                      \
			}                                                                                      \
			else                                                                                   \
			{                                                                                      \
				D += 2 * d.major;                                                                  \
			}                                                                                      \
			minor++;                                                                               \
		}                                                                                          \
	}

PLOTLINE(low, y, x);
PLOTLINE(high, x, y);

void blx_draw_line(t_blx *ctx, t_vi2d p0, t_vi2d p1, t_color col)
{
	if (_abs_sub(p1.y, p0.y) < _abs_sub(p1.x, p0.x))
	{
		if (p0.x > p1.x)
			_line_low(ctx, p1, p0, col);
		else
			_line_low(ctx, p0, p1, col);
	}
	else
	{
		if (p0.y > p1.y)
			_line_high(ctx, p1, p0, col);
		else
			_line_high(ctx, p0, p1, col);
	}
}
