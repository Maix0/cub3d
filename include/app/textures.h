/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:56:28 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/05 14:44:09 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
#define TEXTURES_H

#include "me/types.h"
#include "me/hash/hasher.h"

typedef enum e_texture t_texture;

enum e_texture
{
	TEX_NONE,
	TEX_NORTH,
	TEX_SOUTH,
	TEX_EAST,
	TEX_WEST,
};

void	hash_texture(t_hasher *hasher, t_texture *texture);
bool	cmp_texture(t_texture *lhs, t_texture *rhs);

#endif /* TEXTURES_H */
