/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step0_comments.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cberganz <cberganz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 15:36:20 by cberganz          #+#    #+#             */
/*   Updated: 2022/02/26 16:53:53 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	jump_quotes(char *cmd, int *i)
{
	if (cmd[*i] && cmd[*i] == '\'')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '\'')
			*i += 1;
	}
	if (cmd[*i] && cmd[*i] == '"')
	{
		*i += 1;
		while (cmd[*i] && cmd[*i] != '"')
			*i += 1;
	}
}

void	remove_comments(char **input)
{
	int		i;

	i = 0;
	if ((*input)[i] == '#')
		(*input)[i] = '\0';
	while ((*input)[++i])
	{
		jump_quotes(*input, &i);
		if ((*input)[i] == '#' && ft_ischarset((*input)[i - 1], " \t", NULL))
		{
			(*input)[i] = '\0';
			break ;
		}
	}
}