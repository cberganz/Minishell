/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 18:15:02 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/20 19:04:40 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe_command	*set_pipe_cmd_node(char *pipe_cmd)
{
	t_pipe_command	*pipe_command;

	if (mem_alloc(sizeof(t_pipe_command), pipe_command))
		return (NULL);
	pipe_command->infile = 0;
	pipe_command->infile = 1;
	pipe_command->splitted_cmd = NULL;
	pipe_command->cmd_content = pipe_cmd;
	return (pipe_command);
}

t_list	*set_pipe_commands_list(char *command)
{
	int				i;
	char			**pipe_cmds;
	t_list			*pipe_cmd_list;
	t_list			*tmp;
	t_pipe_command	*cmd_struct;

	i = 0;
	pipe_cmds = ft_split(command, "|");
	if (pipe_cmds == NULL)
		return (NULL);
	while (pipe_cmds[i])
	{
		cmd_struct = set_pipe_cmd_node(pipe_cmds[i]);
		if (!cmd_struct)
			return (NULL);
		tmp = ft_lstnew(cmd_struct);
		if (!tmp)
			return (NULL);
		ft_lstadd_back(&pipe_cmd_list, tmp);
		i++;
	}
	return (pipe_cmd_list);
}

void	single_pipe_parsing(t_list **command_list)
{
	t_list	*tmp;

	tmp = *command_list;
	while (tmp)
	{
		((t_command *)tmp->content)->command_list = set_pipe_commands_list(((t_command *)tmp->content)->command);
		if (!((t_command *)tmp->content)->command_list)
			return (NULL);
		tmp = tmp->next;
	}
}
