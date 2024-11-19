/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 17:38:22 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 14:27:07 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUTS_H
# define INPUTS_H

# include "me/types.h"
# include "me/vec/vec_u8.h"
# include "me/vec2/vec2.h"

# ifndef BLX_H

typedef struct s_blx	t_blx;

# endif

typedef struct s_blx_input
{
	t_vec_u8			keysyms_pressed;
	t_vec_u8			keysyms_held;
	t_vec_u8			keysyms_released;
	t_u8				mouse;
	t_vi2d				mouse_pos;
	t_vi2d				mouse_old_pos;

}						t_blx_input;

/// @brief Create an input manager
/// @param ctx the BLX context
/// @return the created input manager
/// @note this is an internal function, you should not call it yourself
t_error				create_inputs_manager(t_blx *ctx);

#endif
