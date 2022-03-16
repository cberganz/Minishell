/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:22 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/16 15:04:14 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	is_numeric(char *s)
{
	if (*s == '-')
		s++;
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

int	builtin_exit(char **exec_args, int exit)
{
	int	exit_status;

	exit_status = g_status;
	if (!exit)
		ft_putendl_fd("exit", 1);
	if (exec_args[0] && exec_args[1])
	{
		ft_putendl_fd("Minishell: exit: too many arguments", 2);
		return (1);
	}
	else if (exec_args[0])
	{
		if (!is_numeric(exec_args[0]))
		{
			ft_putstr_fd("Minishell: exit: ", 2);
			ft_putstr_fd(exec_args[0], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit_status = 2;
		}
		else
			exit_status = ft_atoi(exec_args[0]);
	}
	free_and_exit(exit_status);
	return (0);
}
