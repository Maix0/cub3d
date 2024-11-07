/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:39:58 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/07 14:02:22 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/types.h"
#include "me/hash/hasher.h"
#include "me/hashmap/hashmap_texture_path.h"
#include "me/hashmap/hashmap_texture.h"

void	free_texture_path_pair(struct s_kv_texture_path pair)
{
	string_free(pair.val);
}

void	free_texture_pair(struct s_kv_texture pair)
{
	blx_sprite_free(pair.val);
}

void	hash_texture(t_hasher *hasher, t_texture *texture)
{
	hasher_write_i32(hasher, *texture);
}

bool	cmp_texture(t_texture *lhs, t_texture *rhs)
{
	return (*lhs == *rhs);
}
