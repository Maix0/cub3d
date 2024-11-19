/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:38:08 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:06:11 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/inputs.h"
#include "me/blx/blx.h"
#include "me/vec/vec_u8.h"
#include <stdio.h>

static t_vec_u8	alloc_input_vector(void)
{
	t_vec_u8	out;

	out = vec_u8_new(64, NULL);
	if (out.buffer == NULL)
		return (out);
	while (out.len < 64)
		vec_u8_push(&out, 0);
	return (out);
}

t_error	create_inputs_manager(t_blx *ctx)
{
	t_blx_input	inputs_manager;

	(void)(ctx);
	inputs_manager = (t_blx_input){
		.mouse = 0,
		.keysyms_held = alloc_input_vector(),
		.keysyms_pressed = alloc_input_vector(),
		.keysyms_released = alloc_input_vector(),
	};
	if (inputs_manager.keysyms_released.buffer == NULL || \
		inputs_manager.keysyms_pressed.buffer == NULL || \
		inputs_manager.keysyms_held.buffer == NULL)
	{
		vec_u8_free(inputs_manager.keysyms_held);
		vec_u8_free(inputs_manager.keysyms_pressed);
		vec_u8_free(inputs_manager.keysyms_released);
		return (ERROR);
	}
	ctx->inputs = inputs_manager;
	return (NO_ERROR);
}

void	free_input_manager(t_blx_input inputs)
{
	vec_u8_free(inputs.keysyms_held);
	vec_u8_free(inputs.keysyms_pressed);
	vec_u8_free(inputs.keysyms_released);
}
