/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 02:20:17 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/01 20:06:29 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec(t_list *cmd_list, char *envp[])
{
	t_list	*command_list;
	int		i;

	i = 0;
	heredoc_management(cmd_list);
	while (cmd_list)
	{
		command_list = ((t_command *)cmd_list->content)->command_list;
		command_parsing(command_list);
		split_args(command_list);
		remove_quotes_list(command_list);
		(void)envp;	
		cmd_list = cmd_list->next;
		i++;
	}
}
