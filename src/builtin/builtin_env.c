/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:12 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/04 01:31:14 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **exec_args, int exit, char **envp[])
{
	int	line;

	line = 0;
	while ((*envp)[line])
	{
		printf("%s\n", (*envp)[line]);
		line++;
	}
	if (exit)
		free_and_exit(0);
	(void)exec_args;
	return (0);
}
