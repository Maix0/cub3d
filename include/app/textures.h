/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:56:28 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/04 13:57:27 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
#define TEXTURES_H

typedef enum e_texture t_texture;

enum e_texture
{
	TEX_NONE,
	TEX_NORTH,
	TEX_SOUTH,
	TEX_EAST,
	TEX_WEST,
};

#endif /* TEXTURES_H */
