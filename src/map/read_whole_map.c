/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_whole_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:09:07 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:28:55 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/fs/fs.h"
#include "me/mem/mem.h"
#include "me/str/str.h"
#include "me/string/string.h"
#include "me/vec/vec_str.h"

/// Read a whole file as lines and puts every lines into the vector. removes the
/// newline at the end if there are some
t_error	read_whole_map(t_fd *file, t_vec_str *out)
{
	t_string	l;
	t_str		trimmed;
	t_vec_str	ret;

	ret = vec_str_new(1, str_free);
	if (ret.buffer == NULL)
		return (ERROR);
	while (!getline_fd(file, &l))
	{
		trimmed = str_trim_end(l.buf, " \t\n\v\r\f");
		if (trimmed == NULL)
			return (vec_str_free(ret), ERROR);
		vec_str_push(&ret, trimmed);
		string_free(l);
		if (ret.buffer == NULL)
			return (mem_free(trimmed), ERROR);
	}
	return (*out = ret, NO_ERROR);
}
