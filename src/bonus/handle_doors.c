/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_doors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 19:24:01 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/25 13:09:07 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/vec2/vec2.h"

// gestion ouverture, ne s'ouvre que si la SPC est released
void	handle_door(t_blx *ctx, t_game *game, t_vi2d pos)
{
	t_tile	door_tile;

	(void)(ctx);
	if (!BONUS)
		return ;
	if (!is_key_released(ctx, KB_space))
		return ;
	door_tile = get_tile(&game->map, pos);
	if (door_tile & TILE_SOLID)
		door_tile &= ~TILE_SOLID;
	else
		door_tile |= TILE_SOLID;
	set_tile(&game->map, pos, door_tile);
}
