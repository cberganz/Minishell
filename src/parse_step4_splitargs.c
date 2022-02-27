/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step4_splitargs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 00:39:47 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/27 01:40:27 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	split_args(t_list *command_list)
{
	char	*command;
	int		i;

	i = 0;
	while (command_list)
	{
		command = ((t_pipe_command *)command_list->content)->cmd_content;
		((t_pipe_command *)command_list->content)->exec_args = ft_split_pipe(command, " ");
		command_list = command_list->next;
	}
}
