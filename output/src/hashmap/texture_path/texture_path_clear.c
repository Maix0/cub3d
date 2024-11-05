/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_texture_path.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:00:22 by maiboyer          #+#    #+#             */
/*   Updated: 2023/12/11 15:24:44 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/types.h"



#include "me/hashmap/hashmap_texture_path.h"
#include "me/mem/mem.h"


void	hmap_texture_path_clear(t_hashmap_texture_path *self)
{
	t_usize					bucket_id;
	t_entry_texture_path		*cur;
	t_entry_texture_path		*next;

	bucket_id = 0;
	while (bucket_id < self->num_buckets)
	{
		cur = self->buckets[bucket_id];
		while (cur != NULL)
		{
			next = cur->next;
			self->free(cur->kv);
			mem_free(cur);
			cur = next;
		}
		bucket_id++;
	}
}
