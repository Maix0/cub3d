/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 16:38:08 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/17 23:12:24 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/blx/blx.h"
#include "me/blx/inputs.h"
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

t_blx_input	create_inputs_manager(t_blx *ctx)
{
	t_blx_input	out;

	(void)(ctx);
	out = (t_blx_input){
		.mouse = 0,
		.keysyms_held = alloc_input_vector(),
		.keysyms_pressed = alloc_input_vector(),
		.keysyms_released = alloc_input_vector(),
	};
	return (out);
}
