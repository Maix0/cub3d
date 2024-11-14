/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:00:11 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/14 11:59:19 by maiboyer         ###   ########.fr       */
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

void init_game(t_game *game)
{
	mem_set_zero(game, sizeof(*game));
	game->textures =
		hmap_texture_new(hash_texture, cmp_texture, free_texture_pair);
	game->map.inner = vec_tile_new(16, NULL);
	game->map.info.textures_path = hmap_texture_path_new(
		hash_texture, cmp_texture, free_texture_path_pair);
	game->speed = 5;
	game->rotate_speed = PI;
}
