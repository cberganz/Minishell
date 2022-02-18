/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:33:06 by cberganz          #+#    #+#             */
/*   Updated: 2021/11/26 14:54:50 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char			*nstr;
	unsigned int	i;

	if (!s)
		return (NULL);
	nstr = ft_strnew(ft_strlen(s));
	if (!nstr)
		return (NULL);
	i = 0;
	while (s[i])
	{
		nstr[i] = f(s[i]);
		i++;
	}
	return (nstr);
}
