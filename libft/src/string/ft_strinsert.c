/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:31:01 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/25 13:11:21 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static uint8_t ft_isquote(int c)
{
	if (c == 39 || c == '"')
		return (1);
	return (0);
}

uint8_t	ft_strinsert(char **src, char *to_insert, int start, int (*func_is)(int))
{
	int		stop;
	char	*nstr;

	stop = 1;
	if (ft_isdigit(*(*src + start + stop)))
		stop++;
	else if (ft_isquote(*(*src + start + stop)))
		;
	else
	{
		while (*(*src + start + stop) && func_is(*(*src + start + stop)))
			stop++;
	}
	if (mem_alloc(ft_strlen(*src) + ft_strlen(to_insert) - stop + 3,
			(void **)&nstr))
		return (-1);
	nstr[0] = '\0';
	ft_strlcpy(nstr, *src, start + 1);
	ft_strlcpy(nstr + start, to_insert, ft_strlen(to_insert) + 1);
	ft_strlcpy(nstr + start + ft_strlen(to_insert),
		*src + start + stop, ft_strlen(*src));
	mem_remove(*src);
	*src = nstr;
	return (0);
}
