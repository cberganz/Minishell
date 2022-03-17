/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/16 23:09:37 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_until_equal(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	write(fd, str, i);
	write(fd, "\"", 1);
}

void	write_after_equal(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	write(fd, &str[i], ft_strlen(&str[i]));
	write(fd, "\"\n", 2);
}

int	export_print_env(char *envp[], int fd)
{
	while (*envp)
	{
		write(fd, "export ", 7);
		write_until_equal(*envp, fd);
		write_after_equal(*envp, fd);
		envp++;
	}
	return (0);
}
