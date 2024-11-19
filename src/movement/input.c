/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:42:31 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:27:43 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/state.h"
#include "me/blx/blx.h"
#include <math.h>

void	perform_collision(t_blx *ctx, t_game *game);
void	handle_mouse_movement(t_blx *ctx, t_game *game);

void	sanitize_input(t_blx *ctx, t_game *game)
{
	(void)(ctx);
	while (game->angle >= PI)
		game->angle -= 2.0 * PI;
	while (game->angle < -PI)
		game->angle += 2.0 * PI;
}

void	movement_keys(t_blx *ctx, t_game *game)
{
	if (is_key_held(ctx, KB_w) || is_key_held(ctx, KB_Up))
	{
		game->new_pos.x += cos(game->angle) * game->speed * ctx->elapsed;
		game->new_pos.y += sin(game->angle) * game->speed * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_s) || is_key_held(ctx, KB_Down))
	{
		game->new_pos.x -= cos(game->angle) * game->speed * ctx->elapsed;
		game->new_pos.y -= sin(game->angle) * game->speed * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_q))
	{
		game->new_pos.x += sin(game->angle) * game->speed * ctx->elapsed;
		game->new_pos.y -= cos(game->angle) * game->speed * ctx->elapsed;
	}
	if (is_key_held(ctx, KB_e))
	{
		game->new_pos.x -= sin(game->angle) * game->speed * ctx->elapsed;
		game->new_pos.y += cos(game->angle) * game->speed * ctx->elapsed;
	}
}

bool	handle_input(t_blx *ctx, t_game *game)
{
	game->new_pos = game->pos;
	movement_keys(ctx, game);
	if (is_key_held(ctx, KB_Escape))
		return (true);
	if (is_key_held(ctx, KB_a) || is_key_held(ctx, KB_Left))
		game->angle -= game->rotate_speed * ctx->elapsed;
	if (is_key_held(ctx, KB_d) || is_key_held(ctx, KB_Right))
		game->angle += game->rotate_speed * ctx->elapsed;
	if (is_key_held(ctx, KB_p))
		game->fov += 0.25 * PI * ctx->elapsed;
	if (is_key_held(ctx, KB_i))
		game->fov -= 0.25 * PI * ctx->elapsed;
	if (is_key_held(ctx, KB_o))
		game->fov = PI / 4;
	if (BONUS && is_key_pressed(ctx, KB_m))
		game->mouse_enable = !game->mouse_enable;
	handle_mouse_movement(ctx, game);
	sanitize_input(ctx, game);
	return (perform_collision(ctx, game), false);
}
