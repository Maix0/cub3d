/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_texture_path.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:00:22 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 15:29:01 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_TEXTURE_PATH_H
# define HASHMAP_TEXTURE_PATH_H

# define DEFAULT_BUCKETS 750

# include "app/textures.h"
# include "me/hash/hasher.h"
# include "me/string/string.h"
# include "me/types.h"
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/// @struct A key-value pair for the hashmap
typedef struct s_kv_texture_path
{
	t_texture					key;
	t_string					val;
}								t_kv_texture_path;

/// @typedef A function that hashes a key
typedef void	(*t_hash_texture_path_fn)(t_hasher *hasher,
						t_texture *key);
/// @typedef A function that drops a key-value pair
typedef void	(*t_free_texture_path_fn)(t_kv_texture_path val);
/// @typedef A function that compares two keys and returns true if they are
/// equal
typedef bool	(*t_eq_texture_path_fn)(t_texture *lhs,
						t_texture *rhs);

/// @struct A single entry in the hashmap
/// @var hash_id The hash of the key
/// @var kv The key-value pair
/// @var next The next entry in the bucket
typedef struct s_entry_texture_path
{
	t_usize						hash_id;
	t_kv_texture_path			kv;
	struct s_entry_texture_path	*next;
}								t_entry_texture_path;

/// @struct A hashmap of keys t_texture and values t_string
/// @var buckets The array of buckets
/// @var num_buckets The number of buckets
/// @var hasher The hasher function
/// @var hfunc The hash function
/// @var cfunc The comparison function
/// @var free The free function
typedef struct s_hashmap_texture_path
{
	t_entry_texture_path		**buckets;
	t_usize						num_buckets;
	t_hasher					hasher;
	t_hash_texture_path_fn		hfunc;
	t_eq_texture_path_fn		cfunc;
	t_free_texture_path_fn		free;
}								t_hashmap_texture_path;

/// @brief Creates a new hashmap with the given hash, comparison, and free
/// functions
/// @param hash The hash function
/// @param cmp The comparison function
/// @param free The free function
/// @return A new hashmap
t_hashmap_texture_path			*hmap_texture_path_new(\
	t_hash_texture_path_fn hash, t_eq_texture_path_fn cmp, \
	t_free_texture_path_fn free);

/// @brief Creates a new hashmap with the given hash, comparison, and free
/// functions
/// @param hash The hash function
/// @param cmp The comparison function
/// @param free The free function
/// @param cap The number of buckets
/// @return A new hashmap
t_hashmap_texture_path			*hmap_texture_path_new_with_buckets(\
	t_hash_texture_path_fn hash, t_eq_texture_path_fn cmp, \
	t_free_texture_path_fn free, size_t cap);

/// @brief Free the hashmap and all of its entries
/// @param hmap The hashmap to free
void							hmap_texture_path_free(\
	t_hashmap_texture_path *hmap);

/// @brief Clear the hashmap, removing all of its entries
/// @param hmap The hashmap to clear
void							hmap_texture_path_clear(\
	t_hashmap_texture_path *hmap);

/// @brief Inserts a key-value pair into the hashmap
/// @param hmap The hashmap
/// @param key The key
/// @param value The value
/// @return true if the key already existed before, false otherwise
bool							hmap_texture_path_insert(\
	t_hashmap_texture_path *hmap, t_texture key, t_string value);

/// @brief Gets the value associated with the key
/// @param hmap The hashmap
/// @param key The key
/// @return The value associated with the key, or NULL if the key is not in the
/// hashmap
t_string						*hmap_texture_path_get(\
	t_hashmap_texture_path *hmap, t_texture *key);

/// @brief Removes the key-value pair from the hashmap
/// @param hmap The hashmap
/// @param key The key
void							hmap_texture_path_remove(\
	t_hashmap_texture_path *hmap, t_texture *key);

/// @brief Get an entry from the hashmap
/// @param hmap The hashmap
/// @param hash The hash of the key
/// @param key The key
/// @param prev The previous entry in the bucket
/// @return The entry, or NULL if the key is not in the hashmap
/// @note this is an internal function
t_entry_texture_path			*hmap_texture_path_get_entry(\
	t_hashmap_texture_path *hmap, t_usize hash, t_texture *key,
									t_entry_texture_path **prev);

/// @brief Iterates over the hashmap and calls the given function for each
/// key-value pair
/// @param self The hashmap
/// @param func The function to call
/// @param ctx The context to pass to the function
/// @return An error code
/// @note The iteration can be stopped by returning an error code from the
/// function
t_error							hmap_texture_path_iter(\
	t_hashmap_texture_path *self, \
									t_error (*func)(t_usize idx,
										const t_texture *key, t_string *val,
										void *ctx), void *ctx);

/// @brief Clone an entire hashmap, using the given function to duplicate the
/// items
/// @param self The hashmap
/// @param func The function to call
/// @param ctx The context to pass to the function
/// @param out The cloned hashmap
/// @return An error code
t_error							hmap_texture_path_clone(\
	t_hashmap_texture_path *self, \
	t_error (*clone)(const t_kv_texture_path *val,
										void *ctx, t_kv_texture_path *out),
									void *ctx, t_hashmap_texture_path **out);

#endif
