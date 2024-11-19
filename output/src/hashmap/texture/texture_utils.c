/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_texture_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:58:20 by maiboyer          #+#    #+#             */
/*   Updated: 2023/12/11 15:35:37 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/hash/sip.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/mem/mem.h"
#include "me/types.h"
#include <stdlib.h>

t_sprite	*hmap_texture_get(t_hashmap_texture *hmap, t_texture *key)
{
	t_usize			hashed_key;
	t_entry_texture	*entry;
	t_entry_texture	*prev;

	if (hmap == NULL)
		return (NULL);
	hmap->hfunc(&hmap->hasher, key);
	hashed_key = hasher_reset_and_finish(&hmap->hasher);
	entry = hmap_texture_get_entry(hmap, hashed_key, key, &prev);
	if (entry == NULL)
		return (NULL);
	return (&entry->kv.val);
}

void	hmap_texture_remove(t_hashmap_texture *hmap, t_texture *key)
{
	t_usize			hashed_key;
	t_entry_texture	*prev;
	t_entry_texture	*entry;

	if (hmap == NULL || key == NULL)
		return ;
	hmap->hfunc(&hmap->hasher, key);
	hashed_key = hasher_reset_and_finish(&hmap->hasher);
	prev = NULL;
	entry = hmap_texture_get_entry(hmap, hashed_key, key, &prev);
	if (entry == NULL)
		return ;
	if (prev == NULL)
		hmap->buckets[hashed_key % hmap->num_buckets] = entry->next;
	else
		prev->next = entry->next;
	hmap->free(entry->kv);
	mem_free(entry);
	hmap->buckets[hashed_key % hmap->num_buckets] = NULL;
}
