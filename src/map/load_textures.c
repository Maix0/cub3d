/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:39:39 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 22:06:55 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/hashmap/hashmap_texture_path.h"

t_error	_load_single(t_usize _idx, const t_texture *id, t_string *path, \
					void *vctx)
{
	t_blx		*ctx;
	t_game		*game;
	t_sprite	spr;

	(void)(_idx);
	ctx = vctx;
	game = ctx->app.data;
	if (blx_sprite_from_xpm(ctx, path->buf, &spr))
		return (cube_error("Failed to load texture: %s", path->buf), ERROR);
	hmap_texture_insert(game->textures, *id, spr);
	return (NO_ERROR);
}

t_error	fetch_textures(t_blx *ctx)
{
	t_game	*game;

	game = ctx->app.data;
	return (hmap_texture_path_iter(game->map.info.textures_path, _load_single, \
									ctx));
}
