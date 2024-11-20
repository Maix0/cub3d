/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:56:28 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/20 13:13:55 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
# define TEXTURES_H

# include "me/hash/hasher.h"
# include "me/types.h"

typedef enum e_texture	t_texture;

enum e_texture
{
	TEX_DOOR,
	TEX_EAST,
	TEX_NONE,
	TEX_NORTH,
	TEX_SOUTH,
	TEX_WEST,
};

bool	cmp_texture(t_texture *lhs, t_texture *rhs);
void	hash_texture(t_hasher *hasher, t_texture *texture);

#endif /* TEXTURES_H */
