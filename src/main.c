/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgasqui <lgasqui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:52:54 by lgasqui           #+#    #+#             */
/*   Updated: 2024/11/25 14:25:01 by lgasqui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app/maps.h"
#include "app/state.h"
#include "app/textures.h"
#include "app/tile.h"
#include "me/blx/blx.h"
#include "me/blx/blx_key.h"
#include "me/mem/mem.h"
#include "me/vec2/vec2.h"
#include <math.h>

int	real_main(int argc, t_str argv[])
{
	t_game		game;
	t_blx		blx;
	t_blx_app	app_data;

	(void)(&argv[argc]);
	if (argc != 2)
		return (cube_error("Usage: %s <map>", argv[0]), 1);
	if (init_game(&game))
		return (cube_error("Failed allocation"), 1);
	mem_set_zero(&blx, sizeof(blx));
	if (parse_map(&game, argv[1]))
		return (game_free(&game), 1);
	app_data = (t_blx_app){.size_x = 900, .size_y = 700, .pixel_size = 1, \
		.title = "Cub3d - Yes", .data = &game};
	if (blx_initialize(game_loop, game_free_blx, app_data, &blx))
		return (cube_error("Failed to init mlx"), game_free(&game), 1);
	if (fetch_textures(&blx))
		game.exit = true;
	return (blx_run(blx), 0);
}

int	main(int argc, char **argv)
{
	me_exit(real_main(argc, argv));
}
