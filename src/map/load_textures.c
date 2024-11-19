/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:39:39 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:30:20 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/hashmap/hashmap_texture_path.h"
#include "me/string/string.h"

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
	t_game		*game;
	t_string	door_string;

	game = ctx->app.data;
	if (BONUS)
	{
		door_string = string_new(16);
		string_push(&door_string, "./textures/door.xpm");
		hmap_texture_path_insert(game->map.info.textures_path, TEX_DOOR, \
									door_string);
	}
	return (hmap_texture_path_iter(game->map.info.textures_path, _load_single, \
								ctx));
}
