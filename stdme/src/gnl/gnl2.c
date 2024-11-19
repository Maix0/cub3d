/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:18:07 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:18:58 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/string/string.h"
#include "me/gnl/gnl.h"
#include "me/mem/mem.h"
#include "me/str/str.h"
#include "me/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

t_static_buffer	*get_next_line_buffer(int fd)
{
	t_usize					index;
	static t_static_buffer	bufs[BUFFER_LENGTH] = {0};

	index = 0;
	while (index < BUFFER_LENGTH && (bufs[index].fd != fd && bufs[index].used))
		index++;
	if (index == BUFFER_LENGTH)
		return (NULL);
	bufs[index].fd = fd;
	bufs[index].used = true;
	return (&bufs[index]);
}

bool	copy_next_line_into_buffer(int fd, t_string *out,
		char *temp_buffer, t_usize amount)
{
	char	*buf;
	char	*newline;
	bool	got_newline;
	t_usize	other_part_len;

	buf = get_next_line_buffer(fd)->buf;
	newline = buf;
	while (*newline != '\n' && *newline && newline < buf + amount)
		newline++;
	got_newline = *newline == '\n';
	other_part_len = amount - (t_usize)(newline - buf + !got_newline);
	if (amount < (t_usize)(newline - buf + !got_newline))
		other_part_len = 0;
	mem_move(temp_buffer, buf, (t_usize)(newline - buf + got_newline));
	buf[(t_usize)(newline - buf)] = 0;
	temp_buffer[(t_usize)(newline - buf + got_newline)] = 0;
	mem_move(buf, newline + 1, other_part_len);
	string_push(out, temp_buffer);
	buf[amount - (t_usize)(newline - buf + got_newline)] = 0;
	return (got_newline);
}
