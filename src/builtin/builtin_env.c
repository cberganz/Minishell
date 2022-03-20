/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:12 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/20 16:42:07 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **exec_args, char **envp[], int exit, int fd)
{
	int	line;

	line = 0;
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
	(void)exec_args;
	return (0);
}
