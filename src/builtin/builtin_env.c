/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:12 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/22 11:17:44 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **exec_args, char **envp[], int exit, int fd)
{
	int	line;

	line = 0;
	if (exec_args && exec_args[0] && exec_args[0][0] == '-' && exec_args[0][1] != '\0')
	{
		ft_putstr_fd("pwd: option invalide -- \'", 2);
		ft_putchar_fd(exec_args[0][1], 2);
		ft_putendl_fd("\'", 2); // anglais
		ft_putendl_fd("Saisissez \" env --help \" pour plus d'informations.", 2);
		if (exit)
			free_and_exit(125);
		return (125);
	}
	while ((*envp)[line])
	{
		if (!strcmp("_=env", (*envp)[line]))
			ft_putendl_fd("_=/usr/bin/env", fd);
		else
			ft_putendl_fd((*envp)[line], fd);
		line++;
	}
	if (exit)
		free_and_exit(0);
	return (0);
}
