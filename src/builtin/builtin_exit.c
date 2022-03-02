/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 00:23:05 by charles           #+#    #+#             */
/*   Updated: 2022/03/02 00:57:46 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	is_numeric(char *s)
{
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

uint8_t	builtin_exit(char **exec_args, int exit)
{
	int	exit_status;

	exit_status = 0;
	printf("exit\n");
	if (exec_args[1])
	{
		printf(RED "Minishell: exit: too many arguments.\n" RESET);
		exit_status = 1;
	}
	else if (exec_args[0])
	{
		if (!is_numeric(exec_args[0]))
		{
			printf(RED "Minishell: exit: %s :not a numeric argument.\n" RESET, exec_args[0]);
			exit_status = 2;
		}
		else
			exit_status = ft_atoi(exec_args[0]);
	}
	if (exit)
		free_and_exit(exit_status);
	return (exit_status);
}
