// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   test_split.c                                       :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2021/11/23 14:48:27 by cberganz          #+#    #+#             */
// /*   Updated: 2022/02/24 16:19:37 by rbicanic         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "libft.h"

// static int	quote_len(char *sub_input, char quote)
// {
// 	int	i;

// 	i = 1;
// 	while (sub_input[i] && sub_input[i] != quote)
// 		i++;
// 	return (i);
// }

// static int	ft_rows(const char *s, char *sep1, char *sep2)
// {
// 	int	rows;

// 	rows = 0;
// 	while (*s)
// 	{
// 		if (*s == '"')
// 			s += quote_len((char *)s, '"') + 1;
// 		if (*s == '\'')
// 			s += quote_len((char *)s, '\'') + 1;
// 		while (ft_strnequ(&(*s), sep1, 2) || ft_strnequ(&(*s), sep2, 2))
// 			s += 2;
// 		if (*s && !ft_strnequ(&(*s), sep1, 2) && !ft_strnequ(&(*s), sep2, 2))
// 			rows++;
// 		while (*s && !ft_strnequ(&(*s), sep1, 2) && !ft_strnequ(&(*s), sep2, 2))
// 			s++;
// 	}
// 	return (rows);
// }

// static void	*mr_propre(char **str_arr)
// {
// 	int	i;

// 	i = -1;
// 	while (str_arr[++i])
// 		mem_remove(str_arr[i]);
// 	mem_remove(str_arr);
// 	return (NULL);
// }

// static int	ft_wordlen(const char *s, char *sep1, char *sep2)
// {
// 	int		i;
// 	// char	*str_end_sep1;
// 	// char	*str_end_sep2;
// 	// char	*str_end;

// 	i = 0;
// 	while (s[i] && s[i + 1])
// 	{
// 		if (s[i] == '"')
// 			i += quote_len((char *)s, '"');
// 		else if (s[i] == '\'')
// 			i += quote_len((char *)s, '\'');
// 		else if (!ft_strncmp(&s[i], sep1, 2) || !ft_strncmp(&s[i], sep2, 2))
// 			return (i);
// 		i++;
// 	}
// 	return (i + 1);
// 	// str_end_sep1 = ft_strstr(s, sep1);
// 	// str_end_sep2 = ft_strstr(s, sep2);
// 	// if (str_end_sep1 < str_end_sep2 && str_end_sep1)
// 	// 	str_end = str_end_sep1;
// 	// else if (str_end_sep2)
// 	// 	str_end = str_end_sep2;
// 	// else
// 	// 	str_end = str_end_sep1;
// 	// if (!str_end)
// 	// 	return (ft_strlen(s));
// 	// else
// 	// 	return (str_end - s);
// }

// char	**ft_split_double(const char *s, char *sep1, char *sep2)
// {
// 	int		i;
// 	int		n;
// 	char	**str_arr;

// 	if (!s || !sep1 || !sep2)
// 		return (NULL);
// 	str_arr = mem_alloc(sizeof(char *) * (ft_rows(s, sep1, sep2) + 1), NULL);
// 	if (!str_arr)
// 		return (NULL);
// 	i = 0;
// 	while (*s)
// 	{
// 		while (ft_strnequ(&(*s), sep1, 2) || ft_strnequ(&(*s), sep2, 2))
// 			s += 2;
// 		if (*s && !ft_strnequ(&(*s), sep1, 2) && !ft_strnequ(&(*s), sep2, 2))
// 		{
// 			n = ft_wordlen(s, sep1, sep2);
// 			str_arr[i++] = ft_substr(s, 0, n);
// 			if (!str_arr[i - 1])
// 				return (mr_propre(str_arr));
// 			s += n;
// 		}
// 	}
// 	str_arr[i] = NULL;
// 	return (str_arr);
// }
