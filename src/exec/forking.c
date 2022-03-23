/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forking.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:33:07 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/23 14:18:52 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_fds(t_pipe_command *command, t_pipe_command *prev,
			t_list *command_list)
{
	if (command->fd_redirection[FD_IN] != 0)
		close(command->fd_redirection[FD_IN]);
	if (command->fd_redirection[FD_OUT] != 1)
		close(command->fd_redirection[FD_OUT]);
	if (command->pid == 0 && command_list->next != NULL)
	{
		close(command->fd_pipe[FD_OUT]);
		close(command->fd_pipe[FD_IN]);
	}
	else if (prev != NULL)
		close(prev->fd_pipe[FD_IN]);
	if (command_list->next != NULL)
		close(command->fd_pipe[FD_OUT]);
}

void	close_all_fds_cmd_list(t_list *command_list)
{
	t_pipe_command	*el;

	while (command_list)
	{
		el = ((t_pipe_command*)command_list->content);
		if (el->fd_redirection[FD_IN] != 0)
			close(el->fd_redirection[FD_IN]);
		if (el->fd_redirection[FD_OUT] != 1)
			close(el->fd_redirection[FD_OUT]);
		if (el->fd_pipe[FD_IN] != 0)
			close(el->fd_pipe[FD_IN]);
		if (el->fd_pipe[FD_OUT] != 0)
			close(el->fd_pipe[FD_OUT]);
		command_list = command_list->next;
	}
}

static void	exec_child(t_pipe_command *command, t_pipe_command *prev,
				t_list *command_list, char **envp[])
{
	int	ret;

	if (command->redirection_error)
		free_and_exit(1);
	if (!prev || command->fd_redirection[FD_IN] != 0)
		dup2(command->fd_redirection[FD_IN], STDIN_FILENO);
	else
		dup2(prev->fd_pipe[FD_IN], STDIN_FILENO);
	if (!command_list->next || command->fd_redirection[FD_OUT] != 1)
		dup2(command->fd_redirection[FD_OUT], STDOUT_FILENO);
	else
		dup2(command->fd_pipe[FD_OUT], STDOUT_FILENO);
	close_all_fds(command, prev, command_list);
	if (!prev && command_list->next)
		handle_shlvl(-1, envp);
	ret = exec_builtin(command, envp, 1, 1);
	if (!ret)
		exec_bin(command, envp);
	if (ret == 1)
		free_and_exit(ret);
}

uint8_t	forking(t_list *command_list, char **envp[])
{
	t_pipe_command	*command;
	t_pipe_command	*prev;

	prev = NULL;
	signal(SIGQUIT, child_sig_handler);
	// signal(SIGINT, sig_void); // VOIR SI CELA FONCTIONNE
	// signal(SIGTSTP, sig_void);
	// signal(SIGQUIT, sig_void);
	while (command_list)
	{
		command = (t_pipe_command *)command_list->content;
		if (command_list->next != NULL)
		{
			if (pipe(command->fd_pipe) == -1)
			{
				errno_file_error("pipe error", 0);
				close_all_fds_cmd_list(command_list);
				return (1);
			}
		}
		command->pid = fork();
		if (command->pid == 0)
			exec_child(command, prev, command_list, envp);
		else if (command->pid < 0)
			print_message("Minishell: Fork() error.\n", RED, 1);
		close_all_fds(command, prev, command_list);
		prev = command;
		command_list = command_list->next;
	}
	return (0);
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
		waitpid(command->pid, &stat, 0);
		if (WIFEXITED(stat))
			g_status = WEXITSTATUS(stat);
		command_list = command_list->next;
	}
}
