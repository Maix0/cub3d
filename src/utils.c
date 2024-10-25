/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:19:50 by maiboyer          #+#    #+#             */
/*   Updated: 2024/10/25 21:23:27 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/fs/fs.h"
#include "me/printf/printf.h"
#include <stdarg.h>

t_error cube_error(t_const_str fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	put_string_fd(get_stderr(), "Error:\n");
	me_veprintf(fmt, &args);
	put_string_fd(get_stderr(), "\n");
	return (ERROR);
}
