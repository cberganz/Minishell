/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 02:20:17 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/14 19:18:14 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	exec_commands(t_list *command_list, char **envp[])
{
	t_pipe_command	*command;

	command = (t_pipe_command *)command_list->content;
	if (is_builtin(command->exec_args[0]) && !command_list->next)
	{
		g_status = exec_builtin(command, envp, 0);
		return ;
	}
	else
	{
		forking(command_list, envp);
		wait_children(command_list);
	}
}

uint8_t	exec_main(t_list *cmd_list, char **envp[])
{
	t_list	*command_list;
	int		i;

	i = 0;
	if (heredoc_management(cmd_list, envp))
		return (1);
	while (cmd_list)
	{
		if (g_status == 0
			|| ft_strequ(((t_command *)cmd_list->content)->control_op, "||")
			|| ((t_command *)cmd_list->content)->control_op == NULL)
		{
			command_list = ((t_command *)cmd_list->content)->command_list;
			command_parsing(command_list, envp);
			if (!cmd_redirection_management(command_list))
			{
				split_args(command_list);
				wildcard(command_list);
				remove_quotes_list(command_list);
				exec_commands(command_list, envp);
			}
		}
		cmd_list = cmd_list->next;
		i++;
	}
	return (0);
}
