/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:53 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/11 06:17:11 by cberganz         ###   ########.fr       */
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

int	close_save_fd(int fd)
{
	if (fd)
		close(fd);
	return (0);
}

int	exec_builtin(t_pipe_command *command, char **envp[], int exit)
{
	// int		save_fd;

	// save_fd = 0;
	if (command->redirection_error)
		return (1);
	// builtin_redirections(command, &save_fd);
	if (ft_strequ(command->exec_args[0], "exit"))
		return (builtin_exit(command->exec_args + 1, exit));
	else if (ft_strequ(command->exec_args[0], "echo"))
		return (builtin_echo(command->exec_args + 1, exit));//, dup2(save_fd, STDOUT_FILENO), close(save_fd));
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
