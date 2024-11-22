/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_path_clone.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:00:22 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 15:32:24 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/hashmap/hashmap_texture_path.h"
#include "me/types.h"

t_error	hmap_texture_path_clone(t_hashmap_texture_path *self,
		t_error (*clone)(const t_kv_texture_path *val, void *ctx,
			t_kv_texture_path *out), void *ctx, t_hashmap_texture_path **out)
{
	t_usize					bucket_id;
	t_entry_texture_path	*cur;
	t_kv_texture_path		kv;
	t_hashmap_texture_path	*ret;

	if (self == NULL)
		return (ERROR);
	bucket_id = 0;
	ret = hmap_texture_path_new_with_buckets(self->hfunc, self->cfunc, \
		self->free, self->num_buckets);
	if (ret == NULL)
		return (ERROR);
	while (bucket_id < self->num_buckets)
	{
		cur = self->buckets[bucket_id++];
		while (cur != NULL)
		{
			if (clone(&cur->kv, ctx, &kv))
				return (hmap_texture_path_free(ret), ERROR);
			hmap_texture_path_insert(ret, kv.key, kv.val);
			cur = cur->next;
		}
	}
	*out = ret;
	return (NO_ERROR);
}
