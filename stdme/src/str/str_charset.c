/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_charset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:12:24 by maiboyer          #+#    #+#             */
/*   Updated: 2024/11/05 15:04:07 by maiboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "me/str/str.h"
#include "me/types.h"

bool str_is_charset(t_const_str self, t_const_str charset)
{
	t_usize i;

	if (self == NULL || charset == NULL)
		return (false);
	i = 0;
	while (self[i] != '\0')
	{
		if (str_find_chr(charset, self[i]) == NULL)
			return (false);
		i++;
	}
	return (true);
}
