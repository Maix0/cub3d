/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 00:48:23 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/05 14:11:38 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/colors.h"
#include "me/blx/sprite.h"
#include "me/num/f64.h"

static t_error sprite_get_pixel_other_bpp(t_sprite *spr, t_vi2d pos, t_color *out)
{
	(void)(spr);
	(void)(pos);
	(void)(out);
	return (ERROR);
}

t_error sprite_get_pixel(t_sprite *spr, t_vi2d pos, t_color *out)
{
	t_u8   *addr;
	t_color col;

	if (!(pos.x >= 0 && pos.x < spr->width && pos.y >= 0 && pos.y < spr->height))
		return (ERROR);
	if (spr->bpp != 32)
		return (sprite_get_pixel_other_bpp(spr, pos, out));
	addr = &(spr->data)[pos.y * spr->line_size + pos.x * 4];
	if (spr->big_endian)
	{
		col.a = addr[0];
		col.r = addr[1];
		col.g = addr[2];
		col.b = addr[3];
	}
	else
	{
		col.a = addr[3];
		col.r = addr[2];
		col.g = addr[1];
		col.b = addr[0];
	}
	*out = col;
	return (NO_ERROR);
}

t_error sprite_get_pixel_normalized(t_sprite *spr, t_vf2d pos, t_color *out)
{
	t_vi2d actual_pos;

	if (spr == NULL)
		return (ERROR);
	pos.x = f64_clamp(0, pos.x, 1);
	pos.y = f64_clamp(0, pos.y, 1);
	actual_pos = vi2d(pos.x * spr->width, pos.y * spr->height);
	return (sprite_get_pixel(spr, actual_pos, out));

}
