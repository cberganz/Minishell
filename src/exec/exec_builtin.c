/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:53 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/14 19:28:00 by rbicanic         ###   ########.fr       */
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

void	builtin_redirections(t_pipe_command *command, int *save_fd)
{

	if (command->fd_redirection[FD_OUT] != 1)
	{
		*save_fd = dup(STDOUT_FILENO);// close ce save in
		dup2(command->fd_redirection[FD_OUT], STDOUT_FILENO);
		close(command->fd_redirection[FD_OUT]);
	}
}

int	restore_stdout(int fd, int exit, int ret)
{
	dup2(fd, STDOUT_FILENO);
	if (fd)
		close(fd);
	if (exit)
		free_and_exit(ret);
	return (0);
}

int	exec_builtin(t_pipe_command *command, char **envp[], int exit)
{
	int		save_fd;
	int		ret;

	save_fd = 0;
	ret = 0;
	if (command->redirection_error)
		return (1);
	builtin_redirections(command, &save_fd);
	if (ft_strequ(command->exec_args[0], "exit"))
		return (restore_stdout(save_fd, 0, 0), builtin_exit(command->exec_args + 1, 0));
	else if (ft_strequ(command->exec_args[0], "echo"))
	{
		ret = builtin_echo(command->exec_args + 1);
		return (restore_stdout(save_fd, exit, ret), ret);
	}
	else if (ft_strequ(command->exec_args[0], "cd"))
	{
		ret = builtin_cd(command->exec_args + 1, envp);
		return (restore_stdout(save_fd, exit, ret), ret);
	}
	else if (ft_strequ(command->exec_args[0], "pwd"))
	{
		ret = builtin_pwd(command->exec_args + 1);
		return (restore_stdout(save_fd, exit, ret), ret);
	}
	else if (ft_strequ(command->exec_args[0], "export"))
	{
		ret = builtin_export(command->exec_args + 1, envp);
		return (restore_stdout(save_fd, exit, ret), ret);
	}
	else if (ft_strequ(command->exec_args[0], "unset"))
	{
		ret = builtin_unset(command->exec_args + 1, envp);
		return (restore_stdout(save_fd, exit, ret), ret);
	}
	else if (ft_strequ(command->exec_args[0], "env"))
	{
		ret = builtin_env(command->exec_args + 1, envp);
		return (restore_stdout(save_fd, exit, ret), ret);
	}
	return (0);
}
