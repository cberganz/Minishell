/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 12:48:01 by cberganz          #+#    #+#             */
/*   Updated: 2021/11/26 10:52:33 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	maxsize;

	if (!s1 || !s2)
		return (NULL);
	maxsize = ft_strlen(s1) + ft_strlen(s2);
	str = ft_strnew(maxsize);
	if (!str)
		return (NULL);
	ft_strlcat(str, s1, maxsize + 1);
	ft_strlcat(str, s2, maxsize + 1);
	return (str);
}
