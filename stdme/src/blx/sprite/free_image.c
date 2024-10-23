/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 22:40:12 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/22 13:36:49 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/blx/sprite.h"
#include <mlx.h>
#include <stdio.h>

void	blx_sprite_free(t_sprite img)
{
	mlx_destroy_image(img.ctx->mlx, img.img);
}
