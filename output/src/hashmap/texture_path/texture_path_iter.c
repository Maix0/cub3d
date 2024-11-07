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

#include "me/hashmap/hashmap_texture_path.h"

t_error hmap_texture_path_iter(t_hashmap_texture_path *self,
								 t_error (*func)(t_usize			 idx,
												 const t_texture *key,
												 t_string *val, void *ctx),
								 void *ctx)
{
	t_usize				 bucket_id;
	t_usize				 all_id;
	t_entry_texture_path *cur;

	bucket_id = 0;
	all_id = 0;
	while (bucket_id < self->num_buckets)
	{
		cur = self->buckets[bucket_id];
		while (cur != NULL)
		{
			if (func(all_id++, &cur->kv.key, &cur->kv.val, ctx))
				return (ERROR);
			cur = cur->next;
		}
		bucket_id++;
	}
	return (NO_ERROR);
}
