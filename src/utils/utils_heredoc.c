/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 00:44:17 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/10 23:41:17 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	check_eof_heredoc(char *input)
{
	if (!input)
	{
		if (g_status == 130)
			return (1);
		else
		{
			printf(RED EOF_HEREDOC RESET);
			return (1);
		}
	}
	return (0);
}

void	close_heredoc_fds(t_list *list)// retours errors ne pas oublier
{
	t_list	*tmp;

	if (!list)
		return ;
	while (list)
	{
		tmp = ((t_command *)list->content)->command_list;
		while (tmp)
		{
			if (((t_pipe_command *)tmp->content)->fd_tmp)
				close(((t_pipe_command *)tmp->content)->fd_tmp);
			tmp = tmp->next;
		}
		list = list->next;
	}
}
