/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 15:17:48 by charles           #+#    #+#             */
/*   Updated: 2022/03/01 17:41:14 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_pipe_command *command, char *envp[])
{
//	if (ft_strequ(exec_args[0], "exit"))
//		builtin_exit(exec_args + 1);
//	else if (ft_strequ(exec_args[0], "echo"))
//		return (builtin_echo(exec_args + 1));
//	else if (ft_strequ(exec_args[0], "cd"))
//		return (builtin_cd(exec_args + 1));
//	else if (ft_strequ(exec_args[0], "pwd"))
//		return (builtin_pwd(exec_args + 1));
//	else if (ft_strequ(exec_args[0], "export"))
//		return (builtin_export(exec_args + 1));
//	else if (ft_strequ(exec_args[0], "unset"))
//		return (builtin_unset(exec_args + 1));
//	else if (ft_strequ(exec_args[0], "env"))
//		return (builtin_env(exec_args + 1));
	(void)command;
	(void)envp;
	return (0);
}

