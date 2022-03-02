/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <cberganz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 15:18:09 by charles           #+#    #+#             */
/*   Updated: 2022/03/02 22:01:49 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	forking(t_list *command_list, char **envp[])
{
	t_pipe_command	*command;
	int				ret;

	while (command_list)
	{
		command = (t_pipe_command *)command_list->content;
		// pipe
		command->pid = fork();
		if (command->pid == 0)
		{
			ret = exec_builtin(command, envp, 1);
			if (!ret)
				exec_bin(command, envp);
			if (ret == 1)
				free_and_exit(ret);
		}
		else if (command->pid < 0)
			print_message("Minishell: Fork() error.\n", RED, -1);
		command_list = command_list->next;
	}
}

void	wait_children(t_list *command_list)
{
	int	exit;
	int(stat);
	t_pipe_command	*command;

	exit = 0;
	stat = 0;
	while (command_list)
	{
		command = (t_pipe_command *)command_list->content;
		waitpid(command->pid, &stat, WEXITSTATUS(stat));
		if (WIFEXITED(stat))
		{
			g_status = WEXITSTATUS(stat);
			if (WEXITSTATUS(stat) == 1)
				free_and_exit(0); // also handle malloc error on child and print message ?
		}
		command_list = command_list->next;
	}
}

