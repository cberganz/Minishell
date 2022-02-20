/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_double.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 14:48:27 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/20 18:55:27 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_rows(const char *s, char *sep1, char *sep2)
{
	int	rows;

	rows = 0;
	while (*s)
	{
		while (ft_strnequ(&(*s), sep1, 2) || ft_strnequ(&(*s), sep2, 2))
			s += 2;
		if (*s && !ft_strnequ(&(*s), sep1, 2) && !ft_strnequ(&(*s), sep2, 2))
			rows++;
		while (*s && !ft_strnequ(&(*s), sep1, 2) && !ft_strnequ(&(*s), sep2, 2))
			s++;
	}
	return (rows);
}

static void	*mr_propre(char **str_arr)
{
	int	i;

	i = -1;
	while (str_arr[++i])
		mem_remove(str_arr[i]);
	mem_remove(str_arr);
	return (NULL);
}

static int	ft_wordlen(const char *s, char *sep1, char *sep2)
{
	char	*str_end_sep1;
	char	*str_end_sep2;
	char	*str_end;

	str_end_sep1 = ft_strstr(s, sep1);
	str_end_sep2 = ft_strstr(s, sep2);
	if (str_end_sep1 < str_end_sep2 && str_end_sep1)
		str_end = str_end_sep1;
	else if (str_end_sep2)
		str_end = str_end_sep2;
	else
		str_end = str_end_sep1;
	if (!str_end)
		return (ft_strlen(s));
	else
		return (str_end - s);
}

char	**ft_split_double(const char *s, char *sep1, char *sep2)
{
	int		i;
	int		n;
	char	**str_arr;

	if (!s || !sep1 || !sep2)
		return (NULL);
	str_arr = mem_alloc(sizeof(char *) * (ft_rows(s, sep1, sep2) + 1), NULL);
	if (!str_arr)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (ft_strnequ(&(*s), sep1, 2) || ft_strnequ(&(*s), sep2, 2))
			s += 2;
		if (*s && !ft_strnequ(&(*s), sep1, 2) && !ft_strnequ(&(*s), sep2, 2))
		{
			n = ft_wordlen(s, sep1, sep2);
			str_arr[i++] = ft_substr(s, 0, n);
			if (!str_arr[i - 1])
				return (mr_propre(str_arr));
			s += n;
		}
	}
	str_arr[i] = NULL;
	return (str_arr);
}
