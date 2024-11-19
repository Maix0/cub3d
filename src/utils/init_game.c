/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:00:11 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 16:58:36 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "app/textures.h"
#include "me/hashmap/hashmap_texture_path.h"
#include "me/mem/mem.h"
#include "me/vec/vec_tile.h"

bool cmp_texture(t_texture *lhs, t_texture *rhs);
void free_texture_pair(struct s_kv_texture pair);
void free_texture_path_pair(struct s_kv_texture_path pair);
void hash_texture(t_hasher *hasher, t_texture *texture);

t_error init_game(t_game *game)
{
	mem_set_zero(game, sizeof(*game));
	game->textures =
		hmap_texture_new(hash_texture, cmp_texture, free_texture_pair);
	if (game->textures == NULL)
		return (ERROR);
	game->map.inner = vec_tile_new(4096, NULL);
	if (game->map.inner.buffer == NULL)
		return (hmap_texture_free(game->textures), ERROR);
	game->map.info.textures_path = hmap_texture_path_new(
		hash_texture, cmp_texture, free_texture_path_pair);
	if (game->map.info.textures_path == NULL)
		return (hmap_texture_free(game->textures),
				vec_tile_free(game->map.inner), ERROR);
	game->str = string_new(1024);
	if (game->str.buf == NULL)
		return (hmap_texture_path_free(game->map.info.textures_path),
				hmap_texture_free(game->textures),
				vec_tile_free(game->map.inner), ERROR);
	game->speed = 5;
	game->rotate_speed = PI;
	game->fov = PI / 4;
	if (BONUS)
		game->mouse_enable = true;
	return (NO_ERROR);
}
