/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:16:15 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:16:15 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/fs/fs.h"
#include "me/string/string.h"
#include "me/gnl/gnl.h"

t_error	getline_fd(t_fd *fd, t_string *out)
{
	bool		error;
	t_string	value;

	if (out == NULL || fd == NULL)
		return (ERROR);
	value = get_next_line(fd->fd, &error);
	if (error)
		return (ERROR);
	return (*out = value, NO_ERROR);
}
