/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 02:11:39 by charles           #+#    #+#             */
/*   Updated: 2022/03/02 02:15:24 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **exec_args, int exit, char *envp[])
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
	if (exit)
		free_and_exit(0);
	(void)exec_args;
	return (0);
}
