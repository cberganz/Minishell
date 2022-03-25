/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step3_tilde.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 00:38:24 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:34:48 by rbicanic         ###   ########.fr       */
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

static uint8_t	flag(char *str)
{
	int	i;

	i = 0;
	jump_quotes(str, &i);
	if (str[i] == '~' && ft_ischarset(str[i + 1], "<>:/ ", NULL))
		return (1);
	while (str[++i])
	{
		jump_quotes(str, &i);
		if (str[i] && str[i] == '~' && ft_ischarset(str[i - 1], "<> ", NULL)
			&& (ft_ischarset(str[i + 1], "<>:/ ", NULL)
				|| str[i + 1] == '\0'))
			return (1);
	}
	return (0);
}

static uint8_t	insert(t_list *command_list, int i, char **envp[])
{
	char	*to_insert;
	char	*command;

	command = ((t_pipe_command *)command_list->content)->cmd_content;
	if (command[i] == '~' && ft_ischarset(command[i - 1], "<> \t", NULL)
		&& (ft_ischarset(command[i + 1], "<>:/ \t", NULL)
			|| command[i + 1] == '\0'))
	{
		to_insert = get_env("HOME", envp);
		if (!to_insert)
			to_insert = "";
		if (ft_strinsert(&command, to_insert, i, 1))
			print_message("Minishell: Allocation error.\n", RED, 1);
		((t_pipe_command *)command_list->content)->cmd_content = command;
		return (1);
	}
	return (0);
}

void	tilde_expansion(t_list *command_list, char **envp[])
{
	int		i;
	char	*command;

	while (command_list)
	{
		command = ((t_pipe_command *)command_list->content)->cmd_content;
		if (command[0] == '~' && (ft_ischarset(command[1], "<>:/ \t", NULL)
				|| command[1] == '\0'))
			insert(command_list, 0, envp);
		while (flag(((t_pipe_command *)command_list->content)->cmd_content))
		{
			i = 0;
			command = ((t_pipe_command *)command_list->content)->cmd_content;
			while (command[++i])
			{
				jump_quotes(command, &i);
				if (insert(command_list, i, envp))
					break ;
			}
		}
		command_list = command_list->next;
	}
}
