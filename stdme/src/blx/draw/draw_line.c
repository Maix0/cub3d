/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:58:47 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:14:25 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"

t_i32	_abs_sub(t_i32 lhs, t_i32 rhs)
{
	t_i32	res;

	res = (lhs - rhs);
	if (res < 0)
		res = -res;
	return (res);
}

void	_swap_vi2d(t_vi2d *lhs, t_vi2d *rhs)
{
	t_vi2d	tmp;

	tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

void	_line_low(t_blx *ctx, t_vi2d p0, t_vi2d p1, t_color col)
{
	t_vi2d	d;
	t_i32	delta;
	t_vi2d	p;
	t_i32	yi;

	d = vi2d(p1.x - p0.x, p1.y - p0.y);
	yi = 1;
	if (d.y < 0)
	{
		yi = -1;
		d.y = -d.y;
	}
	delta = (2 * d.y) - d.x;
	p = p0;
	while (p.x <= p1.x)
	{
		blx_draw(ctx, vi2d(p.x++, p.y), col);
		if (delta > 0)
		{
			p.y += yi;
			delta += 2 * (d.y - d.x);
		}
		else
			delta += 2 * d.y;
	}
}

void	_line_high(t_blx *ctx, t_vi2d p0, t_vi2d p1, t_color col)
{
	t_vi2d	d;
	t_i32	delta;
	t_vi2d	p;
	t_i32	xi;

	d = vi2d(p1.x - p0.x, p1.y - p0.y);
	xi = 1;
	if (d.x < 0)
	{
		xi = -1;
		d.x = -d.x;
	}
	delta = (2 * d.x) - d.y;
	p = p0;
	while (p.y <= p1.y)
	{
		blx_draw(ctx, vi2d(p.x, p.y++), col);
		if (delta > 0)
		{
			p.x += xi;
			delta += 2 * (d.x - d.y);
		}
		else
			delta += 2 * d.x;
	}
}

void	blx_draw_line(t_blx *ctx, t_vi2d p0, t_vi2d p1, t_color col)
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
