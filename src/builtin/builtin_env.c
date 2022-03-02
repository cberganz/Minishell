/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 02:11:39 by charles           #+#    #+#             */
/*   Updated: 2022/03/02 22:18:28 by cberganz         ###   ########.fr       */
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
