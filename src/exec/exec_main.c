/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 02:20:17 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/22 22:42:33 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	set_dollar_underscore(t_list *command_list, char **envp[],
			t_pipe_command *command)
{
	int	i;

	i = 0;
	if (!command->exec_args[0])
		return ;
	while (command->exec_args[i])
		i++;
	if (!command_list->next)
		set_env("_", command->exec_args[i - 1], envp);
}

static void	exec_commands(t_list *command_list, char **envp[])
{
	t_pipe_command	*command;

	command = (t_pipe_command *)command_list->content;
	set_dollar_underscore(command_list, envp, command);
	if (is_builtin(command->exec_args[0]) && !command_list->next)
		g_status = exec_builtin(command, envp, 0, 1);
	else
	{
		forking(command_list, envp);
		wait_children(command_list);
	}
}

static void	parenthesis_priority(t_list **cmd_list, int prev)
{
	*cmd_list = (*cmd_list)->next;
	while (*cmd_list
		&& ((t_command *)(*cmd_list)->content)->nb_parenthesis > prev)
		*cmd_list = (*cmd_list)->next;
}

static uint8_t	perform_exec(t_list *cmd_list, char **envp[])
{
	t_list	*command_list;

	if ((g_status == 0
			&& ft_strequ(((t_command *)cmd_list->content)->control_op, "&&"))
		|| (g_status != 0
			&& ft_strequ(((t_command *)cmd_list->content)->control_op, "||"))
		|| ((t_command *)cmd_list->content)->control_op == NULL)
	{
		command_list = ((t_command *)cmd_list->content)->command_list;
		command_parsing(command_list, envp);
		split_args(command_list);
		wildcard(command_list);
		remove_quotes_list(command_list);
		exec_commands(command_list, envp);
		return (1);
	}
	else
		return (0);
}

uint8_t	exec_main(t_list *cmd_list, char **envp[])
{
	int		prev_nb_parenthesis;

	if (heredoc_management(cmd_list, envp))
		return (1);
	prev_nb_parenthesis = ((t_command *)cmd_list->content)->nb_parenthesis;
	while (cmd_list)
	{
		if (perform_exec(cmd_list, envp))
		{
			prev_nb_parenthesis
				= ((t_command *)cmd_list->content)->nb_parenthesis;
			cmd_list = cmd_list->next;
		}
		else
			parenthesis_priority(&cmd_list, prev_nb_parenthesis);
		if (g_status == 130)
			return (1);
	}
	return (0);
}
