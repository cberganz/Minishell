/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 14:48:27 by cberganz          #+#    #+#             */
/*   Updated: 2021/11/26 10:56:16 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_ptab_len(char const *s, char c)
{
	int	ptab_len;
	int	i;

	if (!s || !c)
		return (-1);
	ptab_len = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
			ptab_len++;
		while (s[i] != c && s[i])
			i++;
	}
	return (ptab_len);
}

static char	*w_create(char const *s, char c)
{
	int		i;
	char	*cpy;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	cpy = malloc((i + 1) * sizeof(char));
	if (!cpy)
		return (NULL);
	cpy[i] = '\0';
	while (--i >= 0)
		cpy[i] = s[i];
	return (cpy);
}

char	**ft_split(char const *s, char c)
{
	char	**ptab;
	int		i;
	int		ptab_count;
	int		ptab_len;

	i = 0;
	ptab_count = 0;
	ptab_len = ft_ptab_len(s, c);
	ptab = malloc((ptab_len + 1) * sizeof(char *));
	if (!ptab)
		return (NULL);
	while (ptab_count < ptab_len)
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
		{
			ptab[ptab_count] = w_create(&s[i], c);
			ptab_count++;
		}
		while (s[i] != c && s[i])
			i++;
	}
	ptab[ptab_count] = NULL;
	return (ptab);
}
