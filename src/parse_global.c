/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_global.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 13:58:56 by rbicanic          #+#    #+#             */
/*   Updated: 2022/02/26 16:49:47 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*global_parsing(char *input)
{
	t_list	*list;

	remove_comments(&input);
	list = parse_step1(input);
	if (!list)
		return (NULL);
	if (!single_pipe_parsing(&list))
		return (NULL);
	return (list);
}

void	command_parsing(t_list *command_list)
{
	tilde_expansion(command_list);
	variable_expansion(command_list);
}
