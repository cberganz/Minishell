/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:31:01 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/25 15:37:49 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

uint8_t	ft_strinsert(char **src, char *to_insert, int start,
			char *charset, int (*func_is)(int))
{
	int		stop;
	char	*nstr;

	stop = 1;
	if (ft_isdigit(*(*src + start + stop)))
		stop++;
	else if (ft_ischarset(*(*src + start + stop), "\'\"", NULL))
		;
	else
	{
		while (*(*src + start + stop) && ft_ischarset(*(*src + start + stop),
				charset, func_is))
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
