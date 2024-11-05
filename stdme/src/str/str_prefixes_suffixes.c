/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_prefixes_suffixes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:31:57 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/04 13:44:33 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/str/str.h"

bool str_endwith(t_const_str self, t_const_str str)
{
	t_usize i;

	if (self == NULL || str == NULL)
		return (false);
	i = 0;
	while (self[i] && str[i] && self[i] == str[i])
		i++;
	return (str[i] == '\0');
}

bool str_startwith(t_const_str self, t_const_str str)
{
	t_usize i;
	t_usize j;

	if (self == NULL || str == NULL)
		return (false);
	i = 0;
	j = 0;
	while (self[i])
		i++;
	while (str[j])
		j++;
	while (i > 0 && j > 0 && self[i] == str[j])
	{
		i--;
		j--;
	}
	if (i > 0 && j == 1)
		return (self[i - 1] == str[j - 1]);
	return (false);
}
