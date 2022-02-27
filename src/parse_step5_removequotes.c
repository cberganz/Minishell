/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step5_removequotes.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 19:18:34 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/27 04:39:52 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes_str(char **str) // To test, found some errors
{
	int	i;

	i = 0;
	while (*(*str + i))
	{
		if ((*(*str + i) == '"' && *(*str + i + 1) == '"')
			|| (*(*str + i) == '\'' && *(*str + i + 1) == '\''))
			ft_strcpy(&(*(*str + i)), &(*(*str + i + 2)));
		else if (*(*str + i) == '"')
		{
			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
			i += quote_len(&(*(*str + i)), '"');
			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
			i--;
		}
		else if (*(*str + i) == '\'')
		{
			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
			i += quote_len(&(*(*str + i)), '\'');
			ft_strcpy(&(*(*str + i)), &(*(*str + i + 1)));
			i--;
		}
		if (*(*str + i))
			i++;
	}
}

void	remove_quotes_list(t_list *command_list)
{
	int	i;

	i = 0;
	while (command_list)
	{
		if (((t_pipe_command *)command_list->content)->exec_args)
		{
			while (((t_pipe_command *)command_list->content)->exec_args[i])
			{
				remove_quotes_str(&((t_pipe_command *)command_list->content)->exec_args[i]);
				i++;
			}
		}
		command_list = command_list->next;
	}
}
