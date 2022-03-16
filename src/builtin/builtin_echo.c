/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:30:55 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/16 18:29:49 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	is_nflag(char *arg)
{
	if (*arg != '-' || *arg == '\0')
		return (0);
	arg++;
	while (*arg && *arg == 'n')
		arg++;
	if (*arg == '\0')
		return (1);
	return (0);
}

int	builtin_echo(char **exec_args)
{
	int	nflag;
	int	save_fd;

	save_fd = 0;
	nflag = 0;
	while (*exec_args && is_nflag(*exec_args))
	{
		nflag = 1;
		exec_args++;
	}
	while (*exec_args && *(exec_args + 1))
	{
		ft_putstr_fd(*exec_args, 1);
		ft_putstr_fd(" ", 1);
		exec_args++;
	}
	if (*exec_args)
		ft_putstr_fd(*exec_args, 1);
	if (!nflag)
		ft_putendl_fd("", 1);
	// if (exit)
	// 	free_and_exit(0);
	return (0);
}
