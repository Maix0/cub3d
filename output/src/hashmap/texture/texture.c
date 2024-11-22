/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:58:20 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 15:52:33 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/hash/hasher.h"
#include "me/hash/sip.h"
#include "me/hashmap/hashmap_texture.h"
#include "me/mem/mem.h"
#include "me/types.h"
#include <stdlib.h>

t_hashmap_texture	*hmap_texture_new(t_hash_texture_fn hfunc,
		t_eq_texture_fn cfunc, t_free_texture_fn free)
{
	return (hmap_texture_new_with_buckets(hfunc, cfunc, free, DEFAULT_BUCKETS));
}

t_hashmap_texture	*hmap_texture_new_with_buckets(t_hash_texture_fn hfunc,
		t_eq_texture_fn cfunc, t_free_texture_fn free, t_usize buckets)
{
	t_hashmap_texture	*hmap;

	hmap = mem_alloc(sizeof(*hmap));
	if (hmap == NULL)
		return (NULL);
	hmap->buckets = mem_alloc_array(buckets, sizeof(t_entry_texture *));
	if (hmap->buckets == NULL)
		return (mem_free(hmap), NULL);
	hmap->num_buckets = buckets;
	hmap->hasher = hasher_sip13_new();
	if (hmap->hasher.hasher == NULL)
		return (mem_free(hmap->buckets), mem_free(hmap), NULL);
	hmap->hfunc = hfunc;
	hmap->cfunc = cfunc;
	return (hmap->free = free, hmap);
}

void	hmap_texture_free(t_hashmap_texture *hmap)
{
	t_usize			index;
	t_entry_texture	*entry;
	t_entry_texture	*tmp;

	if (hmap == NULL)
		return ;
	index = 0;
	while (index < hmap->num_buckets)
	{
		entry = hmap->buckets[index];
		while (entry != NULL)
		{
			hmap->free(entry->kv);
			tmp = entry->next;
			mem_free(entry);
			entry = tmp;
		}
		index++;
	}
	hasher_finish(&hmap->hasher);
	mem_free(hmap->buckets);
	mem_free(hmap);
}

t_entry_texture	*hmap_texture_get_entry(t_hashmap_texture *hmap,
		t_usize hashed_key, t_texture *key, t_entry_texture **prev)
{
	t_entry_texture	*entry;

	entry = hmap->buckets[hashed_key % hmap->num_buckets];
	while (entry != NULL)
	{
		if (!hmap->cfunc(&entry->kv.key, key))
		{
			*prev = entry;
			entry = entry->next;
		}
		else
		{
			return (entry);
		}
	}
	return (NULL);
}

bool	hmap_texture_insert(t_hashmap_texture *hmap, t_texture key,
		t_sprite v)
{
	t_usize			hashed_key;
	t_entry_texture	*prev;
	t_entry_texture	*entry;

	hmap->hfunc(&hmap->hasher, &key);
	hashed_key = hasher_reset_and_finish(&hmap->hasher);
	prev = NULL;
	entry = hmap_texture_get_entry(hmap, hashed_key, &key, &prev);
	if (entry == NULL)
	{
		entry = mem_alloc(sizeof(t_entry_texture));
		if (entry == NULL)
			return (hmap->free((typeof(entry->kv)){.key = key, .val = v}), 0);
		entry->hash_id = hashed_key;
		entry->kv = (t_kv_texture){.key = key, .val = v};
		entry->next = NULL;
		if (prev == NULL)
			hmap->buckets[hashed_key % hmap->num_buckets] = entry;
		else
			prev->next = entry;
		return (false);
	}
	else
		return (hmap->free(entry->kv), entry->kv = \
			(t_kv_texture){.key = key, .val = v}, true);
}
