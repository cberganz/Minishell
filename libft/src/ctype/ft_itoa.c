/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:53:06 by cberganz          #+#    #+#             */
/*   Updated: 2021/11/25 16:29:56 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len_needed(int n)
{
	unsigned int	len;
	unsigned int	nabs;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		len++;
	nabs = ft_abs(n);
	while (nabs > 0)
	{
		len++;
		nabs /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*str;
	unsigned int	len;
	unsigned int	nabs;

	len = len_needed(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n < 0)
		str[0] = '-';
	nabs = ft_abs(n);
	if (nabs == 0)
		str[0] = '0';
	while (--len > 0)
	{
		str[len] = (nabs % 10) + '0';
		nabs /= 10;
	}
	if (n > 0)
		str[0] = (nabs % 10) + '0';
	return (str);
}
