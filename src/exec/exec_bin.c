/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:41 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/11 03:03:53 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_bin(t_pipe_command *command, char **envp[])
{
	char		*path;
	struct stat	stat;

	path = NULL;
	if (ft_ischarset(command->exec_args[0][0], "./", NULL))
	{
		if (command->exec_args[0][1] == '\0')
			path = NULL;
		else
			path = command->exec_args[0];
	}
	else if (command->exec_args[0][0] != '\0')
		path = get_path(command->exec_args, envp);
	if (!path)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(command->exec_args[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_and_exit(127);
	}
	if (lstat(path, &stat) != -1 && (stat.st_mode & S_IFREG)
		&& (stat.st_mode & S_IXUSR))
	{
		execve(path, command->exec_args, *envp);
		perror("minishell: ");
		free_and_exit(1);
	}
}
