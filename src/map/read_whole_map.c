/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_whole_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:09:07 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/05 14:58:56 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/fs/fs.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec/vec_str.h"

/// Read a whole file as lines and puts every lines into the vector. removes the newline at the end
/// if there are some
t_error read_whole_map(t_fd *file, t_vec_str *out)
{
	t_string  l;
	t_vec_str ret;

	ret = vec_str_new(16, str_free);
	while (!getline_fd(file, &l))
	{
		vec_str_push(&ret, str_trim(l.buf, " \t\n\v\r\f"));
		string_free(l);
	}
	return (*out = ret, NO_ERROR);
}
