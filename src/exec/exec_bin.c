/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:41 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/11 01:44:59 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_bin(t_pipe_command *command, char **envp[])
{
	char		*path;
	struct stat	stat;

	path = get_path(command->exec_args, envp);
	if (!path)
	{
		printf(RED "Minishell: command not found: %s\n"
			RESET, command->exec_args[0]);
		free_and_exit(127);
	}
	if (lstat(path, &stat) != -1 && (stat.st_mode & S_IFREG)
		&& (stat.st_mode & S_IXUSR))
	{
		execve(path, command->exec_args, *envp);
		perror(NULL); // set g_status ? How ?
		exit(3); // TEST
	}
}
