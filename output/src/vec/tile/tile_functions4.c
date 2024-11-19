/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_tile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 17:59:28 by maiboyer          #+#    #+#             */
/*   Updated: 2023/12/30 17:59:28 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/mem/mem.h"
#include "me/types.h"
#include "me/vec/vec_tile.h"
#include <stdlib.h>

t_tile *vec_tile_get(t_vec_tile *vec, t_usize i)
{
	if (vec == NULL || vec->buffer == NULL)
		return (NULL);
	if (i < vec->len)
		return (&vec->buffer[i]);
	return (NULL);
}

t_tile *vec_tile_last(t_vec_tile *vec)
{
	if (vec == NULL || vec->buffer == NULL || vec->len == 0)
		return (NULL);
	return (&vec->buffer[vec->len - 1]);
}

void vec_tile_copy_into(t_vec_tile *vec, t_vec_tile *dest)
{
	if (vec == NULL || dest == NULL)
		return ;
	vec_tile_reserve(dest, vec->capacity);
	mem_copy(dest->buffer, vec->buffer, vec->len * sizeof(t_tile));
}
