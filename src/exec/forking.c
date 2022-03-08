/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:33:07 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/08 12:44:44 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	forking(t_list *command_list, char **envp[])
{
	t_pipe_command	*command;
	int				ret;
	t_pipe_command	*prev;

	prev = NULL;
	while (command_list)
	{
		command = (t_pipe_command *)command_list->content;
		if (pipe(command->fd_pipe) == -1)
			print_message("Minishell: Fork() error.\n", RED, 1);
		command->pid = fork();
		if (command->pid == 0)
		{
			if (!prev || command->fd_redirection[STDIN_FILENO] != 0)
				dup2(command->fd_redirection[STDIN_FILENO], STDIN_FILENO);
			else
				dup2(prev->fd_pipe[STDIN_FILENO], STDIN_FILENO);
			if (!command_list->next || command->fd_redirection[STDOUT_FILENO])
				dup2(command->fd_pipe[STDOUT_FILENO], command->fd_redirection[STDOUT_FILENO]);
			else
				dup2(command->fd_pipe[STDOUT_FILENO], STDOUT_FILENO);
			ret = exec_builtin(command, envp, 1);
			if (!ret)
				exec_bin(command, envp);
			if (ret == 1)
				free_and_exit(ret);
		}
		else if (command->pid < 0)
			print_message("Minishell: Fork() error.\n", RED, -1);
		prev = command;
		command_list = command_list->next;
	}
}

void	wait_children(t_list *command_list)
{
	int				exit;
	int				stat;
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
				free_and_exit(0);
		}
		command_list = command_list->next;
	}
}
