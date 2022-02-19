/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 12:47:33 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/19 15:45:51 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	len_start;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	s += start;
	len_start = ft_strlen(s);
	if (len > len_start)
		len = len_start;
	if (mem_alloc(sizeof(char) * (len + 1), (void **)&sub))
		return (NULL);
	i = 0;
	while (i++ < len)
		*sub++ = *s++;
	*sub = '\0';
	return (sub - len);
}
