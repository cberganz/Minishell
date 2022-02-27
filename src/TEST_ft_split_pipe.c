/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TEST_ft_split_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 14:48:27 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/27 01:34:04 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_rows(const char *s, char *sep)
{
	int	rows;

	rows = 0;
	while (*s)
	{
		if (*s == '"')
			s += quote_len((char *)s, '"') + 1;
		else if (*s == '\'')
			s += quote_len((char *)s, '\'') + 1;
		while (ft_strnequ(&(*s), sep, ft_strlen(sep)))
			s += ft_strlen(sep);
		if (*s && !ft_strnequ(&(*s), sep, ft_strlen(sep)))
			rows++;
		while (*s && !ft_strnequ(&(*s), sep, ft_strlen(sep)))
			s += ft_strlen(sep);
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

static int	ft_wordlen(const char *s, char *sep)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"')
			i += quote_len(&((char *)s)[i], '"') + 1;
		else if (s[i] == '\'')
			i += quote_len(&((char *)s)[i], '\'') + 1;
		else if (!ft_strncmp(&s[i], sep, ft_strlen(sep)))
			return (i);
		else
			i++;
	}
	return (i);
}

char	**ft_split_pipe(const char *s, char *sep)
{
	int		i;
	int		n;
	char	**str_arr;

	if (!s || !sep)
		return (NULL);
	str_arr = mem_alloc(sizeof(char *) * (ft_rows(s, sep) + 1), NULL);
	if (!str_arr)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (ft_strnequ(&(*s), sep, ft_strlen(sep)))
			s += ft_strlen(sep);
		if (*s && !ft_strnequ(&(*s), sep, ft_strlen(sep)))
		{
			n = ft_wordlen(s, sep);
			str_arr[i++] = ft_substr(s, 0, n);
			if (!str_arr[i - 1])
				return (mr_propre(str_arr));
			s += n;
		}
	}
	str_arr[i] = NULL;
	return (str_arr);
}
//
//int main(void)
//{
//	char **strs;
//
//	strs = ft_split_pipe("aa", " ");
//	strs = ft_split_pipe("test \"|\" | test 1 '|' | test 2 |\" test3  |\" bwieubriqwebrqwjebr", " ");
//	for (int i = 0; strs[i]; i++)
//	{
//		printf("%s\n", strs[i]);
//	}
//	return (0);
//}
