/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/14 18:33:04 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_until_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	write(1, str, i);
	write(1, "\"", 1);
}

void	write_after_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	write(1, &str[i], ft_strlen(&str[i]));
	write(1, "\"\n", 2);
}

int	export_print_env(char *envp[])
{
	while (*envp)
	{
		write(1, "export ", 7);
		write_until_equal(*envp);
		write_after_equal(*envp);
		envp++;
	}
	return (0);
}
