/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 19:23:33 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 19:25:38 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/blx/colors.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/printf/printf.h"
#include "me/string/string.h"
#include "me/vec/vec_tile.h"
#include "me/vec2/vec2.h"

void	game_print_debuginfo(t_blx *ctx, t_game *game)
{
	me_printf_str(&game->str, "FPS:% 3i\n[a/d] angle:% 3i\n[i/o/p] FOV:% 3i", \
	(int)(1. / ctx->elapsed), (int)(game->angle / (2.0 * PI) * 360), \
	(int)(game->fov / (2.0 * PI) * 360));
	blx_fill_rect(ctx, vi2d(ctx->app.size_x - 8 * 16, 0),
		vi2d(ctx->app.size_x, 3 * 8), new_color(0, 0, 0));
	blx_draw_string(ctx, vi2d(ctx->app.size_x - 8 * 16, 0), game->str.buf,
		new_color(255, 255, 255));
	string_clear(&game->str);
}

bool	game_loop(t_blx *ctx)
{
	t_game	*game;
	t_ray	ray;

	game = ctx->app.data;
	if (game->exit)
		return (true);
	blx_clear(ctx, new_color(0x1E, 0x1E, 0x1E));
	if (handle_input(ctx, game))
		return (true);
	game->door_timer += ctx->elapsed;
	if (game->door_timer >= 4.0)
		game->door_timer = 0.0;
	cast_rays(ctx, game);
	game_print_debuginfo(ctx, game);
	if (BONUS)
	{
		draw_minimap(ctx, game);
		ray = my_ray(game, game->angle, true);
		if (ray.hit_wall && (ray.tile & TILE_DOOR) && ray.ray_len < 1.0 \
			&& ray.ray_len > INC)
			handle_door(ctx, game, ray.tile_pos);
	}
	return (false);
}

void	game_free(t_game *game)
{
	vec_tile_free(game->map.inner);
	hmap_texture_path_free(game->map.info.textures_path);
	hmap_texture_free(game->textures);
	string_free(game->str);
}

void	game_free_blx(t_blx_app app)
{
	game_free(app.data);
}
