/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_tile.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 18:46:53 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/22 15:30:04 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_TILE_H
# define VEC_TILE_H

# include "app/tile.h"
# include "me/types.h"

/// @brief A function that takes two t_tile and compare them
typedef bool						(*t_vec_tile_sort_fn)(t_tile *lhs,
							t_tile *rhs);
/// @brief A function that free an t_tile
typedef void						(*t_free_tile_item)(t_tile elem);

/// @brief A dynamic array of t_tile
typedef struct s_vec_tile			t_vec_tile;

struct								s_vec_tile
{
	t_free_tile_item				free_func;
	t_usize							len;
	t_usize							capacity;
	t_tile							*buffer;
};

struct								s_vec_tile_splice_arguments
{
	t_usize							index;
	t_usize							old_count;
	t_usize							new_count;
	const t_tile					*elements;
};

/// @brief Create a new vec_tile with a given capacity
/// @param capacity The capacity of the new vec_tile (in terms of
/// elements)
/// @param free_function The function that will be used to free the elements of
/// the vec_tile
t_vec_tile							vec_tile_new(t_usize capacity,
										t_free_tile_item free_function);
/// @brief Push an element to the last position of the vec_tile
/// @param vec The vec_tile to push the element to
/// @param element The element to push
t_error								vec_tile_push(t_vec_tile *vec,
										t_tile element);

/// @brief Push an element to the first position of the vec_tile
/// @param vec The vec_tile to push the element to
/// @param element The element to push
/// @note This operation is O(n)
t_error								vec_tile_push_front(t_vec_tile *vec,
										t_tile element);

/// @brief Get the last element from the vec_tile, and remove it from the
/// vec_tile
/// @param vec The vec_tile to get the element from
/// @param[out] out The last element of the vec_tile
/// @return true if the operation failed, false otherwise
t_error								vec_tile_pop(t_vec_tile *vec,
										t_tile *value);

/// @brief Get the first element from the vec_tile, and remove it from
/// the vec_tile
/// @param vec The vec_tile to get the element from
/// @param[out] out The first element of the vec_tile
/// @return true if the operation failed, false otherwise
/// @note This operation is O(n)
t_error								vec_tile_pop_front(t_vec_tile *vec,
										t_tile *value);

/// @brief Free the vector and all its elements
/// @param vec The vec_tile to free
void								vec_tile_free(t_vec_tile vec);

/// @brief Make the vec_tile at least the given capacity
/// @param vec The vec_tile to reserve
/// @param wanted_capacity The minimum capacity to reserve
/// @return true if the operation failed, false otherwise
t_error								vec_tile_reserve(t_vec_tile *vec,
										t_usize wanted_capacity);

/// @brief Run the function and returns the index of the first element that
/// returns true
/// @param vec The vec_tile to search in
/// @param fn The function to run on each element
/// @param[out] index The index of the first element that returns true
t_error								vec_tile_find(t_vec_tile *vec,
										bool (*fn)(const t_tile *),
										t_usize *index);

/// @brief Run the function and returns the index of the first element that
/// returns true, but starting at index starting_index
/// @param vec The vec_tile to search in
/// @param fn The function to run on each element
/// @param starting_index The index to start the search from
/// @param[out] index The index of the first element that returns true
t_error								vec_tile_find_starting(t_vec_tile *vec,
										bool (*fn)(const t_tile *),
										t_usize starting_index, t_usize *index);

/// @brief Run the function on every element of the vec_tile and returns
/// if all elements returned true
/// @param vec The vec_tile to search in
/// @param fn The function to run on each element
/// @param[out] result The result of the operation
/// @return true if the operation failed, false otherwise
/// @note If the vec_tile is empty, result will be true
t_error								vec_tile_all(t_vec_tile *vec,
										bool (*fn)(const t_tile *),
										bool *result);

/// @brief Run the function on every element of the vec_tile and returns
/// if any element returned true
/// @param vec The vec_tile to search in
/// @param fn The function to run on each element
/// @param[out] result The result of the operation
/// @return true if the operation failed, false otherwise
/// @note If the vec_tile is empty, result will be false
t_error								vec_tile_any(t_vec_tile *vec,
										bool (*fn)(const t_tile *),
										bool *result);

/// @brief Run the function on every element of the vec_tile
/// @param vec The vec_tile to iterate over
/// @param fn The function to run on each element
/// @param state The state to pass to the function
void								vec_tile_iter(t_vec_tile *vec,
										void (*fn)(t_usize index, t_tile *value,
											void *state), void *state);

/// @brief Reverse the order of the elements in the vec_tile
/// @param vec The vec_tile to reverse
void								vec_tile_reverse(t_vec_tile *vec);

/// @brief Sort the elements of the vec_tile
/// @param vec The vec_tile to sort
/// @param is_sorted The function to use to compare the elements
void								vec_tile_sort(t_vec_tile *vec,
										t_vec_tile_sort_fn is_sorted);

/// @brief Get a pointer to the last element of the vec_tile
/// @param vec The vec_tile to get the element from
/// @param[out] out A pointer to the last element of the vec_tile
/// @return true if the operation failed, false otherwise
t_error								vec_tile_back(t_vec_tile *vec,
										t_tile **out);

/// @brief Get a pointer to the i'th element, or NULL otherwise
/// @param vec The vec_tile to get the element from
/// @return A pointer to the element or NULL
t_tile								*vec_tile_get(t_vec_tile *vec, t_usize i);

/// @brief Get a pointer to the last element, or NULL otherwise
/// @param vec The vec_tile to get the element from
/// @return A pointer to the last element or NULL
t_tile								*vec_tile_last(t_vec_tile *vec);

/// @brief Perform a simple bytewise copy into the other vector
/// @param vec The vec_tile to be copied from
/// @param dest The vec_tile to be copied to
void								vec_tile_copy_into(t_vec_tile *vec,
										t_vec_tile *dest);

#endif
