/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:53 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/11 02:48:50 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *exec_args)
{
	return (ft_strequ(exec_args, "exit")
		|| ft_strequ(exec_args, "echo")
		|| ft_strequ(exec_args, "cd")
		|| ft_strequ(exec_args, "pwd")
		|| ft_strequ(exec_args, "export")
		|| ft_strequ(exec_args, "unset")
		|| ft_strequ(exec_args, "env"));
}

int	exec_builtin(t_pipe_command *command, char **envp[], int exit)
{
	if (command->redirection_error)
		return (1);
	else if (ft_strequ(command->exec_args[0], "exit"))
		return (builtin_exit(command->exec_args + 1, exit));
	else if (ft_strequ(command->exec_args[0], "echo"))
		return (builtin_echo(command->exec_args + 1, exit));
	else if (ft_strequ(command->exec_args[0], "cd"))
		return (builtin_cd(command->exec_args + 1, exit, envp));
	else if (ft_strequ(command->exec_args[0], "pwd"))
		return (builtin_pwd(command->exec_args + 1, exit));
	else if (ft_strequ(command->exec_args[0], "export"))
		return (builtin_export(command->exec_args + 1, exit, envp));
	else if (ft_strequ(command->exec_args[0], "unset"))
		return (builtin_unset(command->exec_args + 1, exit, envp));
	else if (ft_strequ(command->exec_args[0], "env"))
		return (builtin_env(command->exec_args + 1, exit, envp));
	return (0);
}
