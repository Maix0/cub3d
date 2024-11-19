/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_prefixes_suffixes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:31:57 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/19 17:17:06 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/str/str.h"
#include <stdio.h>

bool	str_startwith(t_const_str self, t_const_str str)
{
	t_usize	i;

	if (self == NULL || str == NULL)
		return (false);
	i = 0;
	while (self[i] && str[i] && self[i] == str[i])
		i++;
	return (str[i] == '\0');
}

bool	str_endwith(t_const_str self, t_const_str str)
{
	t_usize	i;
	t_usize	of;

	if (self == NULL || str == NULL)
		return (false);
	i = str_len(self);
	of = str_len(str);
	if (of > i)
		return (false);
	i -= of;
	return (str_compare(&self[i], str));
}
